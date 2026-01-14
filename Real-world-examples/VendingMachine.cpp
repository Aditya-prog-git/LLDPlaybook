
/*
===========================================================
 VENDING MACHINE – LOW LEVEL DESIGN (STATE PATTERN)
===========================================================

OVERVIEW:
This project implements a vending machine using the
State Design Pattern.

Two versions are included:
1. Simple Vending Machine
   - Single item
   - Fixed price
   - Focus on state transitions

2. Multi-Item Vending Machine
   - Multiple items with different prices & quantities
   - Realistic extension of the same design

-----------------------------------------------------------
STATE DESIGN PATTERN:
The State Pattern allows an object to change its behavior
based on its internal state without using large if-else
or switch statements.

- Context: VendingMachine
- State Interface: VendingState
- Concrete States:
  NO_COIN, HAS_COIN, DISPENSING, SOLD_OUT

Each state:
- Defines valid operations
- Decides the next state

-----------------------------------------------------------
PROBLEM STATEMENT:
Design a vending machine that:
- Accepts coins
- Allows item selection
- Dispenses items
- Handles insufficient balance
- Detects sold-out condition
- Restores service after refill

-----------------------------------------------------------
SOLUTION:
- VendingMachine holds a reference to the current state
- All actions are delegated to the current state object
- State methods return the next state
- Inventory and balance are updated during transitions

This removes complex conditionals and keeps behavior
clean, extensible, and state-driven.
===========================================================
*/


#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

/*
=================================================================
=           SIMPLE (SINGLE ITEM) VENDING MACHINE                =
=           Demonstrates CORE STATE PATTERN                      =
=================================================================
*/
namespace SimpleVM {

class VendingMachine;
class VendingState;

class VendingState {
public:
    virtual VendingState* insertCoin(VendingMachine*, int) = 0;
    virtual VendingState* selectItem(VendingMachine*) = 0;
    virtual VendingState* dispense(VendingMachine*) = 0;
    virtual VendingState* returnCoin(VendingMachine*) = 0;
    virtual VendingState* refill(VendingMachine*, int) = 0;
    virtual string getStateName() = 0;
    virtual ~VendingState() = default;
};

class VendingMachine {
private:
    VendingState* currentState;
    int itemCount;
    int itemPrice;
    int insertedCoins;

    VendingState* noCoinState;
    VendingState* hasCoinState;
    VendingState* dispenseState;
    VendingState* soldOutState;

public:
    VendingMachine(int count, int price);

    //SETTERS
    void insertCoin(int c) { currentState = currentState->insertCoin(this, c); }
    void selectItem() { currentState = currentState->selectItem(this); }
    void dispense() { currentState = currentState->dispense(this); }
    void returnCoin() { currentState = currentState->returnCoin(this); }
    void refill(int q) { currentState = currentState->refill(this, q); }
    
    void decrementItemCount() { itemCount--; }
    void incrementItemCount(int q) { itemCount += q; }
    void setCoins(int c) { insertedCoins = c; }
    void addCoins(int c) { insertedCoins += c; }
    
    //GETTERS
    int getCoins() { return insertedCoins; }
    int getItemCount() { return itemCount; }
    int getPrice() { return itemPrice; }

    VendingState* getNoCoinState() { return noCoinState; }
    VendingState* getHasCoinState() { return hasCoinState; }
    VendingState* getDispenseState() { return dispenseState; }
    VendingState* getSoldOutState() { return soldOutState; }

    void printStatus();
};

class NoCoinState : public VendingState {
public:
    VendingState* insertCoin(VendingMachine* m, int c) override {
        m->setCoins(c);
        cout << "Coin inserted: Rs " << c << endl;
        return m->getHasCoinState();
    }
    VendingState* selectItem(VendingMachine*) override {
        cout << "Insert coin first\n";
        return this;
    }
    VendingState* dispense(VendingMachine*) override {
        cout << "No coin\n";
        return this;
    }
    VendingState* returnCoin(VendingMachine*) override {
        cout << "No coin to return\n";
        return this;
    }
    VendingState* refill(VendingMachine* m, int q) override {
        m->incrementItemCount(q);
        return this;
    }
    string getStateName() override { return "NO_COIN"; }
};

class HasCoinState : public VendingState {
public:
    VendingState* insertCoin(VendingMachine* m, int c) override {
        m->addCoins(c);
        return this;
    }
    VendingState* selectItem(VendingMachine* m) override {
        if (m->getCoins() >= m->getPrice()) {
            m->setCoins(0);
            return m->getDispenseState();
        }
        cout << "Insufficient funds\n";
        return this;
    }
    VendingState* dispense(VendingMachine*) override {
        return this;
    }
    VendingState* returnCoin(VendingMachine* m) override {
        m->setCoins(0);
        return m->getNoCoinState();
    }
    VendingState* refill(VendingMachine*, int) override {
        return this;
    }
    string getStateName() override { return "HAS_COIN"; }
};

class DispenseState : public VendingState {
public:
    VendingState* insertCoin(VendingMachine*, int) override { return this; }
    VendingState* selectItem(VendingMachine*) override { return this; }
    VendingState* dispense(VendingMachine* m) override {
        m->decrementItemCount();
        return (m->getItemCount() > 0)
            ? m->getNoCoinState()
            : m->getSoldOutState();
    }
    VendingState* returnCoin(VendingMachine*) override { return this; }
    VendingState* refill(VendingMachine*, int) override { return this; }
    string getStateName() override { return "DISPENSING"; }
};

class SoldOutState : public VendingState {
public:
    VendingState* insertCoin(VendingMachine*, int) override {
        cout << "Sold out\n";
        return this;
    }
    VendingState* selectItem(VendingMachine*) override { return this; }
    VendingState* dispense(VendingMachine*) override { return this; }
    VendingState* returnCoin(VendingMachine*) override { return this; }
    VendingState* refill(VendingMachine* m, int q) override {
        m->incrementItemCount(q);
        return m->getNoCoinState();
    }
    string getStateName() override { return "SOLD_OUT"; }
};

VendingMachine::VendingMachine(int c, int p) {
    itemCount = c;
    itemPrice = p;
    insertedCoins = 0;

    noCoinState = new NoCoinState();
    hasCoinState = new HasCoinState();
    dispenseState = new DispenseState();
    soldOutState = new SoldOutState();

    currentState = (itemCount > 0) ? noCoinState : soldOutState;
}

void VendingMachine::printStatus() {
    cout << "State: " << currentState->getStateName()
         << " | Items: " << itemCount
         << " | Balance: Rs " << insertedCoins << endl;
}

} // namespace SimpleVM

/*
=================================================================
=            MULTI ITEM VENDING MACHINE (FIXED)                  =
=================================================================
*/
namespace MultiVM {

struct Item {
    string name;
    int price;
    int quantity;
};

class VendingMachine;
class VendingState;

class VendingState {
public:
    virtual VendingState* insertCoin(VendingMachine*, int) = 0;
    virtual VendingState* selectItem(VendingMachine*, const string&) = 0;
    virtual VendingState* dispense(VendingMachine*) = 0;
    virtual VendingState* returnCoin(VendingMachine*) = 0;
    virtual VendingState* refill(VendingMachine*, const string&, int) = 0;
    virtual string getStateName() = 0;
    virtual ~VendingState() = default;
};

class VendingMachine {
private:
    VendingState* currentState;
    unordered_map<string, Item> inventory;
    Item* selectedItem = nullptr;
    int coins = 0;

    VendingState* noCoinState;
    VendingState* hasCoinState;
    VendingState* dispenseState;
    VendingState* soldOutState;

public:
    VendingMachine();

    void addItem(string n, int p, int q) {
        inventory[n] = {n, p, q};
        if (currentState == soldOutState && q > 0)
            currentState = noCoinState;
    }

    unordered_map<string, Item>& getInventory() { return inventory; }
    Item* getSelectedItem() { return selectedItem; }
    void setSelectedItem(Item* i) { selectedItem = i; }

    int getCoins() { return coins; }
    void setCoins(int c) { coins = c; }
    void addCoins(int c) { coins += c; }

    VendingState* getNoCoinState() { return noCoinState; }
    VendingState* getHasCoinState() { return hasCoinState; }
    VendingState* getDispenseState() { return dispenseState; }
    VendingState* getSoldOutState() { return soldOutState; }

    void insertCoin(int c) { currentState = currentState->insertCoin(this, c); }
    void selectItem(const string& n) { currentState = currentState->selectItem(this, n); }
    void dispense() { currentState = currentState->dispense(this); }
    void returnCoin() { currentState = currentState->returnCoin(this); }
    void refill(const string& n, int q) { currentState = currentState->refill(this, n, q); }

    void printStatus();
};

class NoCoinState : public VendingState {
public:
    VendingState* insertCoin(VendingMachine* m, int c) override {
        m->setCoins(c);
        return m->getHasCoinState();
    }
    VendingState* selectItem(VendingMachine*, const string&) override {
        cout << "Insert coin first\n"; return this;
    }
    VendingState* dispense(VendingMachine*) override { return this; }
    VendingState* returnCoin(VendingMachine*) override { return this; }
    VendingState* refill(VendingMachine* m, const string& n, int q) override {
        m->getInventory()[n].quantity += q;
        return this;
    }
    string getStateName() override { return "NO_COIN"; }
};

class HasCoinState : public VendingState {
public:
    VendingState* insertCoin(VendingMachine* m, int c) override {
        m->addCoins(c); return this;
    }
    VendingState* selectItem(VendingMachine* m, const string& n) override {
        auto& inv = m->getInventory();
        if (!inv.count(n)) return this;
        Item& i = inv[n];
        if (i.quantity == 0 || m->getCoins() < i.price) return this;
        m->setSelectedItem(&i);
        return m->getDispenseState();
    }
    VendingState* dispense(VendingMachine*) override { return this; }
    VendingState* returnCoin(VendingMachine* m) override {
        m->setCoins(0);
        return m->getNoCoinState();
    }
    VendingState* refill(VendingMachine*, const string&, int) override { return this; }
    string getStateName() override { return "HAS_COIN"; }
};

class DispenseState : public VendingState {
public:
    VendingState* insertCoin(VendingMachine*, int) override { return this; }
    VendingState* selectItem(VendingMachine*, const string&) override { return this; }
    VendingState* dispense(VendingMachine* m) override {
        Item* i = m->getSelectedItem();
        i->quantity--;
        m->setCoins(0);
        m->setSelectedItem(nullptr);

        for (auto& it : m->getInventory())
            if (it.second.quantity > 0)
                return m->getNoCoinState();

        return m->getSoldOutState();
    }
    VendingState* returnCoin(VendingMachine*) override { return this; }
    VendingState* refill(VendingMachine*, const string&, int) override { return this; }
    string getStateName() override { return "DISPENSING"; }
};

class SoldOutState : public VendingState {
public:
    VendingState* insertCoin(VendingMachine*, int) override { return this; }
    VendingState* selectItem(VendingMachine*, const string&) override { return this; }
    VendingState* dispense(VendingMachine*) override { return this; }
    VendingState* returnCoin(VendingMachine*) override { return this; }
    VendingState* refill(VendingMachine* m, const string& n, int q) override {
        m->getInventory()[n].quantity += q;
        return m->getNoCoinState();
    }
    string getStateName() override { return "SOLD_OUT"; }
};

VendingMachine::VendingMachine() {
    noCoinState = new NoCoinState();
    hasCoinState = new HasCoinState();
    dispenseState = new DispenseState();
    soldOutState = new SoldOutState();

    // ✅ Correct startup state
    currentState = soldOutState;
}

void VendingMachine::printStatus() {
    cout << "State: " << currentState->getStateName()
         << " | Balance: Rs " << coins << endl;
    for (auto& i : inventory)
        cout << "  " << i.first << " Qty: " << i.second.quantity << endl;
}

} // namespace MultiVM

/*
=================================================================
=                           MAIN                                =
=================================================================
*/
int main() {

    /* =========================================================
       SIMPLE VENDING MACHINE : TEST ALL STATES
       States:
       NO_COIN → HAS_COIN → DISPENSING → NO_COIN → SOLD_OUT
       ========================================================= */
    cout << "\n================ SIMPLE VENDING MACHINE ================\n";

    SimpleVM::VendingMachine sm(2, 20);

    // Initial state: NO_COIN
    sm.printStatus();

    // Try invalid actions in NO_COIN
    sm.selectItem();
    sm.dispense();
    sm.returnCoin();

    // Insert insufficient coin
    sm.insertCoin(10);          // NO_COIN → HAS_COIN
    sm.printStatus();

    // Try dispensing without enough money
    sm.selectItem();            // stays HAS_COIN
    sm.dispense();
    sm.returnCoin();            // HAS_COIN → NO_COIN
    sm.printStatus();

    // Insert exact money
    sm.insertCoin(20);          // NO_COIN → HAS_COIN
    sm.selectItem();            // HAS_COIN → DISPENSING
    sm.dispense();              // DISPENSING → NO_COIN
    sm.printStatus();

    // Buy last item
    sm.insertCoin(20);
    sm.selectItem();
    sm.dispense();              // DISPENSING → SOLD_OUT
    sm.printStatus();

    // Test SOLD_OUT behavior
    sm.insertCoin(10);
    sm.selectItem();
    sm.dispense();
    sm.returnCoin();

    // Refill and return to service
    sm.refill(3);               // SOLD_OUT → NO_COIN
    sm.printStatus();

    /* =========================================================
       MULTI ITEM VENDING MACHINE : TEST ALL STATES
       States:
       NO_COIN → HAS_COIN → DISPENSING → NO_COIN → SOLD_OUT
       ========================================================= */
    cout << "\n================ MULTI ITEM VENDING MACHINE ================\n";

    MultiVM::VendingMachine mm;

    // Add inventory
    mm.addItem("Water", 20, 1);
    mm.addItem("Coke", 30, 1);
    mm.addItem("Chips", 15, 2);

    // Initial state: NO_COIN
    mm.printStatus();

    // Try selecting item without coin
    mm.selectItem("Water");
    mm.dispense();
    mm.returnCoin();

    // Insert insufficient coins
    mm.insertCoin(10);          // NO_COIN → HAS_COIN
    mm.selectItem("Coke");      // insufficient funds
    mm.returnCoin();            // HAS_COIN → NO_COIN
    mm.printStatus();

    // Buy Water
    mm.insertCoin(20);
    mm.selectItem("Water");     // HAS_COIN → DISPENSING
    mm.dispense();              // DISPENSING → NO_COIN
    mm.printStatus();

    // Buy Chips (twice)
    mm.insertCoin(15);
    mm.selectItem("Chips");
    mm.dispense();

    mm.insertCoin(15);
    mm.selectItem("Chips");
    mm.dispense();
    mm.printStatus();

    // Buy Coke (last remaining item)
    mm.insertCoin(30);
    mm.selectItem("Coke");
    mm.dispense();              // DISPENSING → SOLD_OUT
    mm.printStatus();

    // Test SOLD_OUT behavior
    mm.insertCoin(10);
    mm.selectItem("Water");
    mm.dispense();
    mm.returnCoin();

    // Refill item and restore service
    mm.refill("Water", 2);      // SOLD_OUT → NO_COIN
    mm.printStatus();

    return 0;
}