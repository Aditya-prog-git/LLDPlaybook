/********************************************************************************************
    SINGLETON DESIGN PATTERN IN C++
    -------------------------------------------------
    A Singleton ensures that only ONE instance of a class exists and provides a global
    access point to it. Use when: A single shared resource / configuration / logger is needed.
********************************************************************************************/


/************************************************************************************************
    1) VERY PRIMITIVE SINGLETON  (Not Recommended)
    - This version does NOT prevent multiple objects.
    - It only warns if more objects are created.
************************************************************************************************/
#include <iostream>
using namespace std;

class PrimitiveSingleton {
private:
    static int instanceCount;  // Tracks how many objects are created

public:
    PrimitiveSingleton() {
        instanceCount++;
        if(instanceCount > 1)
            cout << "[WARNING] More than one instance created: " << instanceCount << endl;
        else
            cout << "[OK] Singleton object created. Instance count: " << instanceCount << endl;
    }
};

int PrimitiveSingleton::instanceCount = 0;  // initialize counter


/************************************************************************************************
    2) BASIC LAZY INITIALIZATION SINGLETON (Recommended for single-threaded programs)
    - Creates the instance only when needed.
    - Prevents multiple objects by making constructor private.
************************************************************************************************/

class LazySingleton {
private:
    static LazySingleton* instance;  // Pointer storing the single instance

    LazySingleton() {                // Private constructor
        cout << "[Lazy] Singleton instance created\n";
    }

public:
    static LazySingleton* getInstance() {
        // Create only ONE object when requested first time
        if(instance == nullptr)
            instance = new LazySingleton();
        return instance;
    }
};

LazySingleton* LazySingleton::instance = nullptr;  // Initialize pointer to NULL


/************************************************************************************************
    3) THREAD-SAFE SINGLETON USING MUTEX (For multi-threaded applications)
    - Uses double-checked locking to avoid race conditions.
    - Ensures safe instance creation even with multiple threads calling getInstance().
************************************************************************************************/
#include <mutex>

class ThreadSafeSingleton {
private:
    static ThreadSafeSingleton* instance;
    static mutex mtx;  // mutex to guard instance creation

    ThreadSafeSingleton() {
        cout << "[Thread-Safe] Singleton instance created\n";
    }

public:
    static ThreadSafeSingleton* getInstance() {

        // First check (fast path, no lock)
        if(instance == nullptr) {

            lock_guard<mutex> lock(mtx); // Lock to prevent race condition

            // Second check inside lock
            if(instance == nullptr)
                instance = new ThreadSafeSingleton();
        }
        return instance;
    }
};

// Define static members
ThreadSafeSingleton* ThreadSafeSingleton::instance = nullptr;
mutex ThreadSafeSingleton::mtx;


/************************************************************************************************
    4) EAGER INITIALIZATION SINGLETON (Instance created at program start)
    - Best when object is always required and startup cost is acceptable.
************************************************************************************************/
class EagerSingleton {
private:
    static EagerSingleton* instance;

    EagerSingleton() {
        cout << "[Eager] Singleton instance created at program start\n";
    }

public:
    static EagerSingleton* getInstance() {
        return instance;  // Always returns the already-created instance
    }
};

// Create instance BEFORE main() runs
EagerSingleton* EagerSingleton::instance = new EagerSingleton();


/************************************************************************************************
    MAIN FUNCTION TO TEST ALL SINGLETON VERSIONS
************************************************************************************************/
int main() {
    cout << "\n================= TESTING PRIMITIVE VERSION =================\n";
    PrimitiveSingleton p1;
    PrimitiveSingleton p2; // Not stopped, only warned


    cout << "\n================= LAZY INITIALIZATION VERSION ================\n";
    LazySingleton* l1 = LazySingleton::getInstance();
    LazySingleton* l2 = LazySingleton::getInstance();
    cout << "LazySingleton same instance? " << (l1 == l2) << "\n";


    cout << "\n================= THREAD-SAFE SINGLETON VERSION ==============\n";
    ThreadSafeSingleton* t1 = ThreadSafeSingleton::getInstance();
    ThreadSafeSingleton* t2 = ThreadSafeSingleton::getInstance();
    cout << "ThreadSafeSingleton same instance? " << (t1 == t2) << "\n";


    cout << "\n================= EAGER INITIALIZATION VERSION ===============\n";
    EagerSingleton* e1 = EagerSingleton::getInstance();
    EagerSingleton* e2 = EagerSingleton::getInstance();
    cout << "EagerSingleton same instance? " << (e1 == e2) << "\n";

    cout << "\n================= END OF NOTES =================\n";
    return 0;
}