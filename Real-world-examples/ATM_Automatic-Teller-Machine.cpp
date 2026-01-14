

/*
===========================================================
 ATM SYSTEM – LOW LEVEL DESIGN (LLD)
===========================================================

OVERVIEW:
This design models a simplified Automated Teller Machine
(ATM) using the State Design Pattern to control user
interaction flow and system behavior.

The ATM is implemented as a state-driven system where
each user action (card insertion, PIN entry, operation
selection, transaction execution) causes a transition
from one state to another.

-----------------------------------------------------------
CORE FUNCTIONALITIES:
The ATM supports the following operations:
- Card insertion and removal
- PIN validation
- Account loading based on card details
- Cash withdrawal
- Balance inquiry
- Card ejection and session cleanup

-----------------------------------------------------------
USER FLOW (HIGH LEVEL):
1. User inserts card
2. ATM moves to HasCard state
3. User enters PIN
4. ATM validates PIN and loads account
5. User selects an operation (Withdraw / Balance Inquiry)
6. ATM processes the transaction
7. Session ends and card is ejected

-----------------------------------------------------------
DESIGN PATTERN USED:
- State Design Pattern

Each ATM state encapsulates:
- Allowed actions
- State-specific behavior
- Valid state transitions

This avoids complex conditional logic and makes the
system easy to extend with new states or operations.

-----------------------------------------------------------
KEY DESIGN DECISIONS:
- ATMMachine owns all Account objects (simplified model)
- ATMInventory is composed within ATMMachine
- ATMState objects are shared and stateless
- Account balance updates and cash dispensing are handled
  atomically with rollback on failure
- User session data (card, account, operation) is cleared
  after each transaction

-----------------------------------------------------------
FAILURE SCENARIOS HANDLED:
- Invalid PIN entry
- Insufficient account balance
- Insufficient ATM cash
- Inability to dispense exact cash amount
- User cancellation during transaction

-----------------------------------------------------------
DESIGN GOALS:
- Clear separation of responsibilities
- Realistic ATM interaction flow
- Safe handling of cash and account balance
- Readable, interview-ready low-level design
- Easy extensibility for new operations or states

===========================================================
*/

#include<iostream>
#include<unordered_map>
#include<vector>

using namespace std;

class Card{
  private:
    string cardNumber;
    int pin;
    string accountNumber;

  public:
    Card(string cardNumber, int pin, string accountNumber) : 
      cardNumber(cardNumber), pin(pin), accountNumber(accountNumber) {};

    string getAccountNumber(){
      return this->accountNumber;
    }

    bool validatePin(int pin){
      return this->pin == pin;
    }
};

class Account{
  private:
    string accountNumber;
    double balance;
  public:
    Account(string accountNumber, double balance) : 
      accountNumber(accountNumber), balance(balance) {};

    string getAccountNumber(){
      return accountNumber;
    }

    double getBalance(){
      return balance;
    }

    bool withdraw(double amount){
      if(amount <= balance){
        balance -= amount;
        return true;
      }
      return false;
    }

    void deposit(double amount){
      balance += amount;
    }
};

enum CashType{
  BILL_100 = 100,
  BILL_50 = 50,
  BILL_20 = 20,
  BILL_10 = 10,
  BILL_5 = 5,
  BILL_1 = 1
};

enum OperationType{
  WITHDRAW,
  BALANCE_INQUIRY
};

class ATMInventory{
  private:
    unordered_map<CashType, int>cashInventory;

  public:
    ATMInventory(){
      cashInventory[CashType::BILL_100] = 10;
      cashInventory[CashType::BILL_50] = 10;
      cashInventory[CashType::BILL_20] = 20;
      cashInventory[CashType::BILL_10] = 30;
      cashInventory[CashType::BILL_5] = 20;
      cashInventory[CashType::BILL_1] = 50;
    }

    bool hasSufficientCash(int amount){
      int value = 0;
      for(auto it : cashInventory)
        value += static_cast<int>(it.first) * it.second;
      return value >= amount;
    }

    unordered_map<CashType, int> dispenseCash(int amount){

      unordered_map<CashType, int>dispensed;
      int remaining = amount;

      CashType order[] = {
        CashType::BILL_100,
        CashType::BILL_50,
        CashType::BILL_20,
        CashType::BILL_10,
        CashType::BILL_5,
        CashType::BILL_1
      };

      for(CashType type : order){
        int value = static_cast<int>(type);
        int count = min(remaining/value, cashInventory[type]);

        if(count > 0){
          dispensed[type] = count;
          remaining -= count * value;
          cashInventory[type] -= count;
        }
      }

      if(remaining > 0){
        for(auto type : dispensed)
          cashInventory[type.first] += type.second;
        return {};
      }
      return dispensed;
    }
};

class ATMState;

class ATMMachine{
  private:
    unordered_map<string, Account*>accounts;

    ATMState* currentState;

    ATMState* idleState;
    ATMState* hasCardState;
    ATMState* pinValidationState;
    ATMState* selectOperationState;
    ATMState* transactionState;

    ATMInventory inventory;
    Card* currentCard;
    Account* currentAccount;
    OperationType currentOperation;

  public:
    //GETTERS
    ATMMachine();

    ATMState* getCurrentState(){
      return currentState;
    }

    ATMState* getIdleState(){
      return idleState;
    }

    ATMState* getHasCardState(){
      return hasCardState;
    }

    ATMState* getSelectOperationState(){
      return selectOperationState;
    }

    ATMState* getPinValidationState(){
      return pinValidationState;
    }

    ATMState* getTransactionState(){
      return transactionState;
    }

    Card* getCurrentCard(){
      return currentCard;
    }

    Account* getCurrentAccount(){
      return currentAccount;
    }

    OperationType getCurrentOperation(){
      return currentOperation;
    }

    ATMInventory& getInventory(){
      return inventory;
    }

    //SETTERS
    void setCard(Card* card){
      currentCard = card;
    }

    bool loadAccountFromCard(){
      string accNo = currentCard->getAccountNumber();
      if(accounts.count(accNo)){
        currentAccount = accounts[accNo];
        return true;
      }
      return false;
    }

    void addAccount(Account* account){
      accounts[account->getAccountNumber()] = account;
    }

    void setCurrentState(ATMState* state){
      currentState = state;
    }

    void setOperation(OperationType operation){
      currentOperation = operation;
    }

    void clearSession(){
      currentCard = nullptr;
      currentAccount = nullptr;
    }
};

class ATMState{
  public:
    ~ATMState(){};
    virtual ATMState* insertCard(ATMMachine*) = 0;
    virtual ATMState* removeCard(ATMMachine*) = 0;
    virtual ATMState* selectOperation(ATMMachine*, OperationType&) = 0;
    virtual ATMState* transactionState(ATMMachine*) = 0;
    virtual string getStateName() = 0;
};

class IdleState : public ATMState{
  public:
    ATMState* insertCard(ATMMachine* state) override {
      cout<<"Card Inserted Successfully!!"<<endl;
      return state->getHasCardState();
    }

    ATMState* removeCard(ATMMachine* state) override {
      cout<<"Insert Card First"<<endl;
      return this;
    }

    ATMState* selectOperation(ATMMachine* state, OperationType &operation) override {
      cout<<"Insert Card First"<<endl;
      return this;
    }

    ATMState* transactionState(ATMMachine* state) override {
      cout<<"Select Operation First"<<endl;
      return this;
    }

    string getStateName() override {
      return "Idle_State";
    }
};

class HasCardState : public ATMState{
  public:
    ATMState* insertCard(ATMMachine* state) override {
      cout<<"Card Already Inserted!!"<<endl;
      return this;
    }

    ATMState* removeCard(ATMMachine* state) override {
      cout<<"Card Removed"<<endl;
      return state->getIdleState();
    }

    ATMState* selectOperation(ATMMachine* state, OperationType &operation) override {
      cout<<"Proceeding to PIN Validation"<<endl;
      return state->getPinValidationState();
    }

    ATMState* transactionState(ATMMachine* state) override {
      cout<<"Select Operation First"<<endl;
      return state->getIdleState();
    }

    string getStateName() override {
      return "Has_Card_State";
    }
};

class PinValidationState : public ATMState{
  public:
    ATMState* insertCard(ATMMachine* state) override {
      cout<<"Card Already Inserted"<<endl;
      return this;
    }

    ATMState* removeCard(ATMMachine* state) override {
      cout<<"Card Removed"<<endl;
      return state->getIdleState();
    }

    ATMState* selectOperation(ATMMachine* state, OperationType &operation) override {
      int PIN;
      cout<<"Enter PIN : ";
      cin>>PIN;

      if(state->getCurrentCard()->validatePin(PIN)){
        if(!state->loadAccountFromCard()){
          cout<<"Account Not Found"<<endl;
          return state->getIdleState();
        }
        state->setOperation(operation);
        return state->getSelectOperationState();
      }

      return this;
    }

    ATMState* transactionState(ATMMachine* state) override {
      cout<<"Select Operation First"<<endl;
      return this;
    }

    string getStateName() override {
      return "PIN_Validation_State";
    }
};

class SelectOperationState : public ATMState{
  public:
    ATMState* insertCard(ATMMachine* state) override {
      cout<<"Card Already Inserted"<<endl;
      return this;
    }

    ATMState* removeCard(ATMMachine* state) override {
      cout<<"Transaction Cancelled"<<endl;
      state->clearSession();
      return state->getIdleState();
    }

    ATMState* selectOperation(ATMMachine* state, OperationType &operation) override {
      state->setOperation(operation);
      return state->getTransactionState();
    }

    ATMState* transactionState(ATMMachine* state) override {
      cout<<"Select Operation First"<<endl;
      return this;
    }

    string getStateName() override {
      return "Select_Operation_State";
    }
};

class TransactionState : public ATMState{
  public:
    ATMState* insertCard(ATMMachine* state) override {
      cout<<"Card Already Inserted!!"<<endl;
      return this;
    }

    ATMState* removeCard(ATMMachine* state) override {
      cout<<"Transaction Failed, Card Removed"<<endl;
      return this;
    }

    ATMState* selectOperation(ATMMachine* state, OperationType &operation) override {
      cout<<"Operation Already Selected, Processing Transaction"<<endl;
      return this;
    }

    ATMState* transactionState(ATMMachine* state) override {
      OperationType type = state->getCurrentOperation();

      if (type == OperationType::WITHDRAW) {
        int amount = 0;
        cout << "Enter Amount To Withdraw : ";
        cin >> amount;

        Account* account = state->getCurrentAccount();

        if (account->getBalance() < amount) {
            cout << "Insufficient Balance in Your Account" << endl;
            return this;
        }

        if (!state->getInventory().hasSufficientCash(amount)) {
            cout << "Not Sufficient Cash in Inventory" << endl;
            return this;
        }

        account->withdraw(amount);

        auto cash = state->getInventory().dispenseCash(amount);
        if (cash.empty()) {
            cout << "Cannot Dispense Exact Amount" << endl;
            account->deposit(amount);   // rollback
            return this;
        }

        cout << "Cash Dispensed Successfully" << endl;
      }
      else if (type == OperationType::BALANCE_INQUIRY) {
          cout << "Current Balance : "
              << state->getCurrentAccount()->getBalance()
              << endl;
      }

      state->clearSession();
      return state->getIdleState();
    }

    string getStateName() override {
      return "Transaction_State";
    }
};

ATMMachine::ATMMachine(){
  idleState = new IdleState();
  hasCardState = new HasCardState();
  pinValidationState = new PinValidationState();
  selectOperationState = new SelectOperationState();
  transactionState = new TransactionState();

  currentState = idleState;
  currentCard = nullptr;
  currentAccount = nullptr;
};

int main() {

    cout << "\n========= ATM SYSTEM TEST CASES =========\n";

    // ---------- Setup ----------
    ATMMachine atm;

    // Accounts
    Account acc1("ACC001", 5000);   // normal
    Account acc2("ACC002", 100);    // low balance
    Account acc3("ACC003", 0);      // zero balance
    Account acc4("ACC004", 10000);  // high balance
    Account acc5("ACC005", 50);     // edge case

    atm.addAccount(&acc1);
    atm.addAccount(&acc2);
    atm.addAccount(&acc3);
    atm.addAccount(&acc4);
    atm.addAccount(&acc5);

    // Cards
    Card card1("CARD001", 1111, "ACC001");
    Card card2("CARD002", 2222, "ACC002");
    Card card3("CARD003", 3333, "ACC003");
    Card card4("CARD004", 4444, "ACC004");
    Card card5("CARD005", 5555, "ACC005");

    OperationType withdraw = WITHDRAW;
    OperationType balance  = BALANCE_INQUIRY;

    // =====================================================
    cout << "\n--- CASE 1: Successful Withdrawal ---\n";
    atm.setCurrentState(atm.getIdleState());
    atm.setCard(&card1);

    atm.setCurrentState(atm.getCurrentState()->insertCard(&atm));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    atm.setCurrentState(atm.getCurrentState()->transactionState(&atm));


    // =====================================================
    cout << "\n--- CASE 2: Wrong PIN ---\n";
    atm.setCurrentState(atm.getIdleState());
    atm.setCard(&card1);

    atm.setCurrentState(atm.getCurrentState()->insertCard(&atm));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    // Enter WRONG PIN here when prompted

    // =====================================================
    cout << "\n--- CASE 3: Insufficient Balance ---\n";
    atm.setCard(&card2);
    atm.setCurrentState(atm.getCurrentState()->insertCard(&atm));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    atm.setCurrentState(atm.getCurrentState()->transactionState(&atm));

    // =====================================================
    cout << "\n--- CASE 4: Zero Balance Account ---\n";
    atm.setCard(&card3);
    atm.setCurrentState(atm.getCurrentState()->insertCard(&atm));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    atm.setCurrentState(atm.getCurrentState()->transactionState(&atm));

    // =====================================================
    cout << "\n--- CASE 5: Balance Inquiry (High Balance Account) ---\n";
    atm.setCard(&card4);
    atm.setCurrentState(atm.getCurrentState()->insertCard(&atm));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, balance));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, balance));
    atm.setCurrentState(atm.getCurrentState()->transactionState(&atm));

    // =====================================================
    cout << "\n--- CASE 6: Edge Case Small Balance ---\n";
    atm.setCard(&card5);
    atm.setCurrentState(atm.getCurrentState()->insertCard(&atm));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    atm.setCurrentState(atm.getCurrentState()->selectOperation(&atm, withdraw));
    atm.setCurrentState(atm.getCurrentState()->transactionState(&atm));

    // =====================================================
    cout << "\n--- CASE 7: User Cancels Transaction ---\n";
    atm.setCard(&card4);
    atm.setCurrentState(atm.getCurrentState()->insertCard(&atm));
    atm.setCurrentState(atm.getCurrentState()->removeCard(&atm));

    cout << "\n========= ALL TEST CASES COMPLETED =========\n";

    return 0;
}