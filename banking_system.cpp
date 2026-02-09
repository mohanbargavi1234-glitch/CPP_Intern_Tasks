#include <iostream>
#include <string>
using namespace std;

// ---------------- Transaction ----------------
class Transaction {
public:
    string type;
    double amount;
    string fromAcc, toAcc;

    Transaction() {}
    Transaction(string t, double a) : type(t), amount(a) {}

    void display() const {
        cout << "Type: " << type << ", Amount: " << amount << endl;
    }
};

// ---------------- Customer ----------------
class Customer {
    string name;
    string phone;
public:
    Customer() {}
    Customer(string n, string p) : name(n), phone(p) {}

    void display() const {
        cout << "Customer: " << name << " (Phone: " << phone << ")" << endl;
    }
};

// ---------------- Account ----------------
class Account {
protected:
    string accNumber;
    double balance;
    Customer customer;
    Transaction trans[100];
    int transCount;

public:
    Account(string num, Customer cust, double bal)
        : accNumber(num), customer(cust), balance(bal), transCount(0) {}

    virtual ~Account() {}

    string getAccNumber() const {
        return accNumber;
    }

    virtual bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            trans[transCount++] = Transaction("WITHDRAW", amount);
            cout << "Withdrew " << amount << ". New balance: " << balance << endl;
            return true;
        }
        cout << "Insufficient balance or invalid amount!" << endl;
        return false;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            trans[transCount++] = Transaction("DEPOSIT", amount);
            cout << "Deposited " << amount << ". New balance: " << balance << endl;
        }
    }

    bool transfer(double amount, Account &target) {
        if (withdraw(amount)) {
            target.deposit(amount);
            trans[transCount - 1].type = "TRANSFER";
            trans[transCount - 1].fromAcc = accNumber;
            trans[transCount - 1].toAcc = target.getAccNumber();
            return true;
        }
        return false;
    }

    void displayInfo() const {
        customer.display();
        cout << "Account: " << accNumber << ", Balance: " << balance << endl;
    }

    void displayRecentTransactions(int count = 5) const {
        cout << "Recent " << count << " Transactions:" << endl;
        int start = (transCount > count) ? transCount - count : 0;
        for (int i = start; i < transCount; i++) {
            trans[i].display();
        }
        if (transCount == 0)
            cout << "No transactions yet." << endl;
    }
};

// ---------------- Savings Account ----------------
class SavingsAccount : public Account {
public:
    SavingsAccount(string num, Customer cust, double bal)
        : Account(num, cust, bal) {}

    bool withdraw(double amount) override {
        const double MIN_BAL = 500.0;
        if (amount > 0 && amount <= (balance - MIN_BAL)) {
            balance -= amount;
            trans[transCount++] = Transaction("WITHDRAW", amount);
            cout << "Withdrew " << amount << ". New balance: " << balance << endl;
            return true;
        }
        cout << "Insufficient (min 500) or invalid amount!" << endl;
        return false;
    }
};

// ---------------- Bank ----------------
class Bank {
    Account* accounts[100];
    int accCount;

public:
    Bank() : accCount(0) {}

    ~Bank() {
        for (int i = 0; i < accCount; i++)
            delete accounts[i];
    }

    void createAccount() {
        string name, phone, accNum;
        double initBal;

        cout << "Customer name: ";
        cin >> name;
        cout << "Phone: ";
        cin >> phone;
        cout << "Account number: ";
        cin >> accNum;
        cout << "Initial balance: ";
        cin >> initBal;

        Customer cust(name, phone);

        char type;
        cout << "Savings (S) or Regular (R)? ";
        cin >> type;

        Account* acc;
        if (type == 'S' || type == 's')
            acc = new SavingsAccount(accNum, cust, initBal);
        else
            acc = new Account(accNum, cust, initBal);

        accounts[accCount++] = acc;
        cout << "Account created!" << endl;
    }

    Account* findAccount(string accNum) {
        for (int i = 0; i < accCount; i++) {
            if (accounts[i]->getAccNumber() == accNum)
                return accounts[i];
        }
        return nullptr;
    }

    void deposit() {
        string accNum;
        double amt;
        cout << "Account number: ";
        cin >> accNum;
        Account* acc = findAccount(accNum);
        if (acc) {
            cout << "Amount: ";
            cin >> amt;
            acc->deposit(amt);
        } else {
            cout << "Account not found!" << endl;
        }
    }

    void withdraw() {
        string accNum;
        double amt;
        cout << "Account number: ";
        cin >> accNum;
        Account* acc = findAccount(accNum);
        if (acc) {
            cout << "Amount: ";
            cin >> amt;
            acc->withdraw(amt);
        } else {
            cout << "Account not found!" << endl;
        }
    }

    void transfer() {
        string fromAcc, toAcc;
        double amt;

        cout << "From account: ";
        cin >> fromAcc;
        cout << "To account: ";
        cin >> toAcc;
        cout << "Amount: ";
        cin >> amt;

        Account* from = findAccount(fromAcc);
        Account* to = findAccount(toAcc);

        if (from && to && from != to) {
            if (from->transfer(amt, *to))
                cout << "Transfer successful!" << endl;
            else
                cout << "Transfer failed!" << endl;
        } else {
            cout << "Invalid accounts!" << endl;
        }
    }

    void displayInfo() {
        string accNum;
        cout << "Account number: ";
        cin >> accNum;
        Account* acc = findAccount(accNum);
        if (acc) {
            acc->displayInfo();
            acc->displayRecentTransactions();
        } else {
            cout << "Account not found!" << endl;
        }
    }
};

// ---------------- Main ----------------
int main() {
    Bank bank;
    int choice;

    do {
        cout << "\n=== Banking System ===" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Transfer" << endl;
        cout << "5. Display Info & Transactions" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: bank.createAccount(); break;
        case 2: bank.deposit(); break;
        case 3: bank.withdraw(); break;
        case 4: bank.transfer(); break;
        case 5: bank.displayInfo(); break;
        case 0: cout << "Thank you!" << endl; break;
        default: cout << "Invalid choice!" << endl;
        }
    } while (choice != 0);

    return 0;
}