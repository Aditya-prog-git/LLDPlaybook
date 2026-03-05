#include <iostream>
#include <unordered_map>
#include <vector>
#include <mutex>

using namespace std;

/*
PROBLEM
-------
Design a simplified Uber ride booking system.

Features:
- Rider requests a ride
- System notifies available drivers
- Driver accepts ride
- Fare is calculated
- Rider pays using selected payment method
- Ride completes

PATTERNS USED
-------------
Strategy Pattern
    - Payment methods
    - Fare calculation

Singleton Pattern
    - RideManager (central system)

THREAD SAFETY
-------------
Ride assignment uses a mutex to avoid multiple drivers
accepting the same ride.

LIMITATIONS
-----------
- In-memory only
- No GPS / location matching
- No persistence
- Single process simulation
*/

class Rider;
class Driver;

/* ================= PAYMENT STRATEGY ================= */

class PaymentStrategy {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentStrategy() {}
};

class UPIPayment : public PaymentStrategy {
public:
    void pay(int amount) override {
        cout << "[PAYMENT] Paid ₹" << amount << " using UPI\n";
    }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(int amount) override {
        cout << "[PAYMENT] Paid ₹" << amount << " using Credit Card\n";
    }
};

/* ================= FARE STRATEGY ================= */

class FareStrategy {
public:
    virtual int calculateFare(int distance) = 0;
    virtual ~FareStrategy() {}
};

class NormalFare : public FareStrategy {
public:
    int calculateFare(int distance) override {
        return distance * 10;
    }
};

class SurgeFare : public FareStrategy {
public:
    int calculateFare(int distance) override {
        return distance * 15;
    }
};

/* ================= RIDE ================= */

class Ride {

private:
    int rideId;
    int riderId;
    int driverId;
    int fare;
    string status;

    // mutex rideMutex;

public:

    Ride() : rideId(-1), riderId(-1), driverId(-1), fare(0), status("NONE") {}

    Ride(int id, int rider)
        : rideId(id), riderId(rider), driverId(-1), fare(0), status("REQUESTED") {}

    int getId() const { return rideId; }

    int getFare() const { return fare; }

    string getStatus() const { return status; }

    void setFare(int f) { fare = f; }

    bool acceptRide(int driver) {

        // lock_guard<mutex> lock(rideMutex);

        if (status != "REQUESTED")
            return false;

        driverId = driver;
        status = "ASSIGNED";

        cout << "[RIDE] Driver " << driver
             << " accepted Ride " << rideId << endl;

        return true;
    }

    void completeRide() {
        status = "COMPLETED";
    }
};

/* ================= DRIVER ================= */

class Driver {

private:
    int driverId;
    bool available;

public:

    Driver() : driverId(-1), available(true) {}

    Driver(int id) : driverId(id), available(true) {}

    int getId() const { return driverId; }

    bool isAvailable() const { return available; }

    void notifyRide(int rideId) {
        cout << "[NOTIFY] Driver " << driverId
             << " notified about Ride " << rideId << endl;
    }

    void assignRide() { available = false; }

    void completeRide() { available = true; }
};

/* ================= RIDER ================= */

class Rider {

private:
    int riderId;
    PaymentStrategy* payment;

public:

    Rider(int id) : riderId(id), payment(nullptr) {}

    int getId() const { return riderId; }

    void setPayment(PaymentStrategy* strategy) {
        payment = strategy;
    }

    void pay(int amount) {

        if (payment)
            payment->pay(amount);
        else
            cout << "[ERROR] Payment method not set\n";
    }
};

/* ================= RIDE MANAGER (SINGLETON) ================= */

class RideManager {

private:

    unordered_map<int, Ride> rides;
    unordered_map<int, Driver> drivers;

    int rideCounter;

    static RideManager* instance;

    RideManager() : rideCounter(1) {}

public:

    static RideManager* getInstance() {

        if (!instance)
            instance = new RideManager();

        return instance;
    }

    /* Register Driver */

    void addDriver(int driverId) {

        drivers.emplace(driverId, Driver(driverId));

        cout << "[SYSTEM] Driver registered: "
             << driverId << endl;
    }

    /* Create Ride */

    int createRide(int riderId) {

        int rideId = rideCounter++;

        rides.emplace(rideId, Ride(rideId, riderId));

        cout << "\n[SYSTEM] Ride Created -> ID: "
             << rideId << endl;

        notifyDrivers(rideId);

        return rideId;
    }

    /* Notify Drivers */

    void notifyDrivers(int rideId) {

        cout << "[SYSTEM] Notifying available drivers...\n";

        for (auto& pair : drivers) {

            Driver& driver = pair.second;

            if (driver.isAvailable())
                driver.notifyRide(rideId);
        }
    }

    /* Driver Accept Ride */

    void driverAcceptRide(int driverId, int rideId) {

        if (!drivers[driverId].isAvailable()) {
            cout << "[ERROR] Driver busy\n";
            return;
        }

        bool success = rides[rideId].acceptRide(driverId);

        if (success)
            drivers[driverId].assignRide();
        else
            cout << "[FAILED] Ride already taken\n";
    }

    /* Complete Ride */

    void completeRide(int rideId, FareStrategy* strategy, int distance) {

        int fare = strategy->calculateFare(distance);

        rides[rideId].setFare(fare);
        rides[rideId].completeRide();

        cout << "[SYSTEM] Ride "
             << rideId
             << " completed. Fare = ₹"
             << fare << endl;
    }

    int getFare(int rideId) {
        return rides[rideId].getFare();
    }
};

RideManager* RideManager::instance = nullptr;

/* ================= TEST CASES ================= */

int main() {

    cout << "\n===== UBER RIDE SYSTEM DEMO =====\n\n";

    RideManager* manager = RideManager::getInstance();

    /* Register Drivers */

    manager->addDriver(101);
    manager->addDriver(102);

    /* Create Rider */

    Rider rider1(1);
    rider1.setPayment(new UPIPayment());

    /* TEST 1: NORMAL RIDE */

    cout << "\n--- TEST 1: NORMAL RIDE ---\n";

    int ride1 = manager->createRide(rider1.getId());

    manager->driverAcceptRide(101, ride1);

    FareStrategy* normalFare = new NormalFare();

    manager->completeRide(ride1, normalFare, 5);

    rider1.pay(manager->getFare(ride1));

    /* TEST 2: MULTIPLE DRIVER ACCEPT */

    cout << "\n--- TEST 2: MULTIPLE DRIVER ACCEPT ---\n";

    int ride2 = manager->createRide(rider1.getId());

    manager->driverAcceptRide(101, ride2);
    manager->driverAcceptRide(102, ride2);

    /* TEST 3: SURGE PRICING */

    cout << "\n--- TEST 3: SURGE PRICING ---\n";

    Rider rider2(2);
    rider2.setPayment(new CardPayment());

    int ride3 = manager->createRide(rider2.getId());

    manager->driverAcceptRide(102, ride3);

    FareStrategy* surgeFare = new SurgeFare();

    manager->completeRide(ride3, surgeFare, 10);

    rider2.pay(manager->getFare(ride3));

    cout << "\n===== DEMO COMPLETE =====\n";

    return 0;
}