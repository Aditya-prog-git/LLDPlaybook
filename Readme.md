<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=rect&color=0:0f2027,50:203a43,100:2c5364&height=90&section=header&text=Low%20Level%20Design%20Practice&fontSize=28&fontColor=ffffff&animation=fadeIn" />
</p>

<p align="center">
  <b>Design Patterns • Clean Architecture • Interview-Ready LLD in C++</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-0A66C2?style=flat-square" />
  <img src="https://img.shields.io/badge/Domain-Low%20Level%20Design-7B2CBF?style=flat-square" />
  <img src="https://img.shields.io/badge/Focus-Clean%20OOP%20%26%20Patterns-16A34A?style=flat-square" />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Patterns-Strategy%20%7C%20Factory%20%7C%20Singleton%20%7C%20Observer%20%7C%20State-F59E0B?style=flat-square" />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Target-Fresher%20%26%20Junior%20Engineers-22C55E?style=flat-square" />
  <img src="https://img.shields.io/badge/Style-Interview%20Explainable-3B82F6?style=flat-square" />
</p>

---

### 👋 What this repository is about

A **curated collection of Low Level Design (LLD) implementations in C++**, focused on **real interview problems** and **core design patterns**.

Built with the intent to:
- Think in **patterns, not if-else**
- Identify **change-prone areas**
- Write **clean, extensible OOP code**
- Explain designs clearly in interviews

> Minimal. Intentional. Interview-ready.

---

## 🧩 Design Patterns Covered

| Pattern | Core Idea |
|-------|-----------|
| **Strategy** | Encapsulate interchangeable behavior |
| **Factory** | Centralize and abstract object creation |
| **Singleton** | Maintain a single shared instance |
| **Observer** | Enable event-driven communication |
| **State** | Alter object behavior based on internal state |

> ℹ️ The **State Pattern** is applied implicitly in real-world systems such as **ATM** and **Vending Machine** to manage state-dependent behavior transitions.

> 💡 These four patterns alone cover a **majority of fresher-level LLD interview scenarios**.

---

## 🗂️ Repository Structure

```text
.
├── factory/
│   ├── factory_basic_pattern.cpp
│   └── factoryDesignPattern.pdf
│
├── observer/
│   ├── observer_basic_pattern.cpp
│   └── real_world_example.pdf
│
├── singleton/
│   └── singleton_basic_pattern.cpp
│
├── strategy/
│   ├── strategy_basic_pattern.cpp
│   ├── strategy_payment.cpp
│   ├── strategy_sorting.cpp
│   ├── strategy_payment.exe
│   └── sorting.exe
│
├── real_world_examples/
│   ├── ATM_Automatic_Teller_Machine.cpp
│   ├── ParkingLot.cpp
│   ├── VendingMachine.cpp
│   ├── PubSubSystem.cpp     
│   └── RideBookingSystem.cpp   
│
├── .gitignore
└── README.md
```
📌 **Each folder is self-contained and can be explored independently.**

---

## 🧪 Pattern-Wise Implementations

### 🔹 Strategy Pattern
**📂 Location:** `strategy/`

**Use Cases Implemented:**
- Payment methods (UPI / Card)
- Sorting algorithms (runtime selection)

**Why Strategy?**  
Used when **business logic varies**, but the overall workflow remains constant.

---

### 🔹 Factory Pattern
**📂 Location:** `factory/`

**Use Cases Implemented:**
- Centralized object creation
- Input-based object selection

**Why Factory?**  
Prevents object creation logic from spreading across the codebase.

---

### 🔹 Singleton Pattern
**📂 Location:** `singleton/`

**Use Cases Implemented:**
- Shared resource management

**Why Singleton?**  
Used when a **single source of truth** is required (configuration, cache, DB manager).

---

### 🔹 Observer Pattern
**📂 Location:** `observer/`

**Use Cases Implemented:**
- Event notification system
- Publisher–subscriber relationship

**Why Observer?**  
Ideal for **event-driven architectures** where components should remain loosely coupled.

---

### 🔹 State Pattern
**📂 Location:** `real_world_examples/`

**Use Cases Implemented:**
- ATM operation flow (Idle → CardInserted → Authenticated → Transaction → Exit)
- Vending machine lifecycle (Idle → Selection → Payment → Dispense)

**Why State?**  
Used when an object’s **behavior changes based on its internal state**, allowing state-specific logic to be isolated and transitions to be handled cleanly.

---

## 🏗️ Real-World LLD Implementations

### 1️⃣ Vending Machine
**📄 File:** `real_world_examples/VendingMachine.cpp`  
**Patterns Used:** Factory, Strategy, Singleton, State

**Key Design Decisions:**
- Product creation via Factory
- Pricing logic via Strategy
- Inventory managed via Singleton
- State-driven flow for machine operations

---

### 2️⃣ Parking Lot System
**📄 File:** `real_world_examples/ParkingLot.cpp`  
**Patterns Used:** Factory, Strategy, Singleton

**Key Design Decisions:**
- Vehicle-based slot allocation
- Flexible pricing models
- Centralized parking state management

---

### 3️⃣ ATM System
**📄 File:** `real_world_examples/ATM_Automatic_Teller_Machine.cpp`  
**Patterns Used:** Strategy, Singleton, State

**Key Design Decisions:**
- Transaction rules encapsulated as strategies
- State-based handling of ATM operations
- Account data managed centrally

---

### 4️⃣ Pub/Sub System
**📄 File:** `real_world_examples/PubSubSystem.cpp`  
**Patterns Used:** Observer, Singleton

**Key Design Decisions:**
- Decoupled publishers and subscribers
- Centralized broker to manage subscriptions
- Event-based message delivery

---

### 5️⃣ Ride Booking System (Uber-lite)
**📄 File:** `real_world_examples/RideBookingSystem.cpp`  
**Patterns Used:** Strategy, Factory, Singleton, Observer

**Key Design Decisions:**
- Fare calculation via Strategy
- Payment method selection via Factory
- Centralized ride lifecycle management
- Driver notification using Observer pattern

---

## 🧠 Interview Readiness

This repository prepares you to confidently:
- Explain **why a pattern was chosen**
- Identify **extension points**
- Discuss **design trade-offs**
- Walk through an **LLD solution step-by-step**

**Sample interview explanation:**
> *“I used the Strategy pattern here because pricing rules change frequently, and this allows new rules to be added without modifying the core business flow.”*

---

## ▶️ How to Run

1. Navigate to any folder  
2. Compile the `.cpp` file:
   ```bash
   g++ filename.cpp -o output
   ```
3. Run the executable:
   ```bash
   ./output
   ```
No external dependencies required. 

## 🔮 Future Enhancements

The following enhancements can be added to further improve design depth and realism:

- Add **Adapter Pattern** examples for third-party integrations
- Improve **CLI interaction flows** for better usability
- Add **basic unit tests** for critical components
- Include **class diagrams** to visualize object relationships
- Extend real-world systems with additional business rules

> ℹ️ **Note:**  
> The **State Pattern** has already been applied implicitly in systems like **ATM** and **Vending Machine** to handle state-based behavior transitions.

---

## 👤 Author

**Aditya**  
Computer Science Engineering  
Focused on Backend Development, Low Level Design & Scalable Systems

> *“Design patterns are not about complexity — they are about controlling change.”*



