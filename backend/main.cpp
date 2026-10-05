#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include <ctime>

using namespace std;

// ==================== ACCOUNT STRUCTURE ====================

struct Account {
    long long accountNumber;
    string name;
    int age;
    string phone;
    string pin;
    double balance;
};

// ==================== FILE NAMES ====================

const string ACCOUNT_FILE = "accounts.txt";
const string TRANSACTION_FILE = "transactions.txt";

// ==================== INPUT CLEAR ====================

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ==================== CURRENT DATE & TIME ====================

string getCurrentTime() {

    time_t now = time(0);
    tm *localTime = localtime(&now);

    stringstream ss;

    ss << setfill('0')
       << setw(2) << localTime->tm_mday << "-"
       << setw(2) << localTime->tm_mon + 1 << "-"
       << localTime->tm_year + 1900 << " "
       << setw(2) << localTime->tm_hour << ":"
       << setw(2) << localTime->tm_min << ":"
       << setw(2) << localTime->tm_sec;

    return ss.str();
}

// ==================== PIN VALIDATION ====================

bool validPIN(string pin) {

    if (pin.length() != 6) {
        return false;
    }

    for (char c : pin) {

        if (!isdigit(c)) {
            return false;
        }
    }

    return true;
}

// ==================== LOAD ACCOUNTS ====================

vector<Account> loadAccounts() {

    vector<Account> accounts;

    ifstream file(ACCOUNT_FILE);

    if (!file) {
        return accounts;
    }

    Account acc;
    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string accountNumber;
        string age;
        string balance;

        getline(ss, accountNumber, '|');
        getline(ss, acc.name, '|');
        getline(ss, age, '|');
        getline(ss, acc.phone, '|');
        getline(ss, acc.pin, '|');
        getline(ss, balance, '|');

        if (!accountNumber.empty()) {

            acc.accountNumber = stoll(accountNumber);
            acc.age = stoi(age);
            acc.balance = stod(balance);

            accounts.push_back(acc);
        }
    }

    file.close();

    return accounts;
}

// ==================== SAVE ACCOUNTS ====================

void saveAccounts(const vector<Account>& accounts) {

    ofstream file(ACCOUNT_FILE);

    for (const Account& acc : accounts) {

        file << acc.accountNumber << "|"
             << acc.name << "|"
             << acc.age << "|"
             << acc.phone << "|"
             << acc.pin << "|"
             << fixed << setprecision(2)
             << acc.balance << "\n";
    }

    file.close();
}

// ==================== GENERATE ACCOUNT NUMBER ====================

long long generateAccountNumber(const vector<Account>& accounts) {

    long long number = 1001;

    for (const Account& acc : accounts) {

        if (acc.accountNumber >= number) {
            number = acc.accountNumber + 1;
        }
    }

    return number;
}

// ==================== FIND ACCOUNT ====================

int findAccount(
    const vector<Account>& accounts,
    long long accountNumber
) {

    for (int i = 0; i < accounts.size(); i++) {

        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }

    return -1;
}

// ==================== VERIFY PIN ====================

bool verifyPIN(const Account& account) {

    string enteredPIN;

    cout << "Enter 6-digit PIN: ";
    cin >> enteredPIN;

    if (enteredPIN == account.pin) {
        return true;
    }

    cout << "Incorrect PIN!\n";

    return false;
}

// ==================== SAVE TRANSACTION ====================

void saveTransaction(
    long long accountNumber,
    string type,
    double amount,
    double balance
) {

    ofstream file(TRANSACTION_FILE, ios::app);

    file << accountNumber << "|"
         << getCurrentTime() << "|"
         << type << "|"
         << fixed << setprecision(2)
         << amount << "|"
         << balance << "\n";

    file.close();
}

// ==================== CREATE ACCOUNT ====================

void createAccount(vector<Account>& accounts) {

    Account acc;

    acc.accountNumber = generateAccountNumber(accounts);

    cout << "\n========== CREATE ACCOUNT ==========\n";

    cout << "Enter your name: ";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    getline(cin, acc.name);

    cout << "Enter your age: ";
    cin >> acc.age;

    if (cin.fail() || acc.age <= 0) {

        clearInput();

        cout << "Invalid age!\n";

        return;
    }

    cout << "Enter phone number: ";
    cin >> acc.phone;

    // ==================== PIN SETUP ====================

    while (true) {

        cout << "Set your 6-digit PIN: ";
        cin >> acc.pin;

        if (validPIN(acc.pin)) {
            break;
        }

        cout << "Invalid PIN!\n";
        cout << "PIN must contain exactly 6 digits.\n";
    }

    acc.balance = 0;

    accounts.push_back(acc);

    saveAccounts(accounts);

    cout << "\n====================================\n";
    cout << "Account created successfully!\n";
    cout << "Your Account Number: "
         << acc.accountNumber << "\n";
    cout << "Initial Balance: ₹0.00\n";
    cout << "====================================\n";
}

// ==================== DEPOSIT ====================

void depositMoney(vector<Account>& accounts) {

    long long accountNumber;
    double amount;

    cout << "\n========== DEPOSIT MONEY ==========\n";

    cout << "Enter account number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1) {

        cout << "Account not found!\n";

        return;
    }

    if (!verifyPIN(accounts[index])) {
        return;
    }

    cout << "Enter amount to deposit: ";
    cin >> amount;

    if (cin.fail() || amount <= 0) {

        clearInput();

        cout << "Invalid amount!\n";

        return;
    }

    accounts[index].balance += amount;

    saveAccounts(accounts);

    saveTransaction(
        accountNumber,
        "DEPOSIT",
        amount,
        accounts[index].balance
    );

    cout << "\nAmount deposited successfully!\n";

    cout << "New Balance: ₹"
         << fixed << setprecision(2)
         << accounts[index].balance << "\n";
}

// ==================== WITHDRAW ====================

void withdrawMoney(vector<Account>& accounts) {

    long long accountNumber;
    double amount;

    cout << "\n========== WITHDRAW MONEY ==========\n";

    cout << "Enter account number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1) {

        cout << "Account not found!\n";

        return;
    }

    if (!verifyPIN(accounts[index])) {
        return;
    }

    cout << "Enter amount to withdraw: ";
    cin >> amount;

    if (cin.fail() || amount <= 0) {

        clearInput();

        cout << "Invalid amount!\n";

        return;
    }

    if (amount > accounts[index].balance) {

        cout << "Insufficient balance!\n";

        return;
    }

    accounts[index].balance -= amount;

    saveAccounts(accounts);

    saveTransaction(
        accountNumber,
        "WITHDRAW",
        amount,
        accounts[index].balance
    );

    cout << "\nAmount withdrawn successfully!\n";

    cout << "Remaining Balance: ₹"
         << fixed << setprecision(2)
         << accounts[index].balance << "\n";
}

// ==================== CHECK BALANCE ====================

void checkBalance(const vector<Account>& accounts) {

    long long accountNumber;

    cout << "\n========== CHECK BALANCE ==========\n";

    cout << "Enter account number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1) {

        cout << "Account not found!\n";

        return;
    }

    if (!verifyPIN(accounts[index])) {
        return;
    }

    cout << "\nAccount Number: "
         << accounts[index].accountNumber << "\n";

    cout << "Name: "
         << accounts[index].name << "\n";

    cout << "Current Balance: ₹"
         << fixed << setprecision(2)
         << accounts[index].balance << "\n";
}

// ==================== ACCOUNT DETAILS ====================

void accountDetails(const vector<Account>& accounts) {

    long long accountNumber;

    cout << "\n========== ACCOUNT DETAILS ==========\n";

    cout << "Enter account number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1) {

        cout << "Account not found!\n";

        return;
    }

    if (!verifyPIN(accounts[index])) {
        return;
    }

    const Account& acc = accounts[index];

    cout << "\n-----------------------------------\n";
    cout << "Account Number : " << acc.accountNumber << "\n";
    cout << "Name           : " << acc.name << "\n";
    cout << "Age            : " << acc.age << "\n";
    cout << "Phone          : " << acc.phone << "\n";
    cout << "Balance        : ₹"
         << fixed << setprecision(2)
         << acc.balance << "\n";
    cout << "-----------------------------------\n";
}

// ==================== TRANSFER MONEY ====================

void transferMoney(vector<Account>& accounts) {

    long long sender;
    long long receiver;
    double amount;

    cout << "\n========== TRANSFER MONEY ==========\n";

    cout << "Enter your account number: ";
    cin >> sender;

    int senderIndex = findAccount(accounts, sender);

    if (senderIndex == -1) {

        cout << "Sender account not found!\n";

        return;
    }

    if (!verifyPIN(accounts[senderIndex])) {
        return;
    }

    cout << "Enter receiver account number: ";
    cin >> receiver;

    int receiverIndex = findAccount(accounts, receiver);

    if (receiverIndex == -1) {

        cout << "Receiver account not found!\n";

        return;
    }

    if (sender == receiver) {

        cout << "You cannot transfer money to the same account!\n";

        return;
    }

    cout << "Enter amount: ";
    cin >> amount;

    if (cin.fail() || amount <= 0) {

        clearInput();

        cout << "Invalid amount!\n";

        return;
    }

    if (amount > accounts[senderIndex].balance) {

        cout << "Insufficient balance!\n";

        return;
    }

    accounts[senderIndex].balance -= amount;

    accounts[receiverIndex].balance += amount;

    saveAccounts(accounts);

    saveTransaction(
        sender,
        "TRANSFER SENT",
        amount,
        accounts[senderIndex].balance
    );

    saveTransaction(
        receiver,
        "TRANSFER RECEIVED",
        amount,
        accounts[receiverIndex].balance
    );

    cout << "\nTransfer successful!\n";

    cout << "₹"
         << fixed << setprecision(2)
         << amount
         << " transferred successfully.\n";
}

// ==================== TRANSACTION HISTORY ====================

void transactionHistory() {

    long long accountNumber;

    cout << "\n========== TRANSACTION HISTORY ==========\n";

    cout << "Enter account number: ";
    cin >> accountNumber;

    ifstream file(TRANSACTION_FILE);

    if (!file) {

        cout << "No transactions found.\n";

        return;
    }

    string line;

    bool found = false;

    cout << "\n";

    cout << left
         << setw(22) << "Date & Time"
         << setw(20) << "Type"
         << setw(12) << "Amount"
         << "Balance\n";

    cout << "-------------------------------------------------------------\n";

    while (getline(file, line)) {

        stringstream ss(line);

        string accNo;
        string date;
        string type;
        string amount;
        string balance;

        getline(ss, accNo, '|');
        getline(ss, date, '|');
        getline(ss, type, '|');
        getline(ss, amount, '|');
        getline(ss, balance, '|');

        if (stoll(accNo) == accountNumber) {

            found = true;

            cout << left
                 << setw(22) << date
                 << setw(20) << type
                 << setw(12) << amount
                 << balance << "\n";
        }
    }

    file.close();

    if (!found) {

        cout << "No transactions found for this account.\n";
    }
}

// ==================== CLOSE ACCOUNT ====================

void closeAccount(vector<Account>& accounts) {

    long long accountNumber;

    cout << "\n========== CLOSE ACCOUNT ==========\n";

    cout << "Enter account number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1) {

        cout << "Account not found!\n";

        return;
    }

    if (!verifyPIN(accounts[index])) {
        return;
    }

    if (accounts[index].balance != 0) {

        cout << "Account cannot be closed.\n";

        cout << "Please withdraw or transfer your remaining balance first.\n";

        return;
    }

    accounts.erase(accounts.begin() + index);

    saveAccounts(accounts);

    cout << "\nAccount closed successfully!\n";
}

// ==================== MAIN MENU ====================

void showMenu() {

    cout << "\n\n";

    cout << "=========================================\n";
    cout << "       BANK MANAGEMENT SYSTEM\n";
    cout << "=========================================\n";

    cout << "1. Create Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. Check Balance\n";
    cout << "5. Account Details\n";
    cout << "6. Transfer Money\n";
    cout << "7. Transaction History\n";
    cout << "8. Close Account\n";
    cout << "9. Exit\n";

    cout << "=========================================\n";
}

// ==================== MAIN FUNCTION ====================

int main() {

    vector<Account> accounts = loadAccounts();

    int choice;

    cout << "\n";
    cout << "=========================================\n";
    cout << "   WELCOME TO BANK MANAGEMENT SYSTEM\n";
    cout << "=========================================\n";

    while (true) {

        showMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {

            clearInput();

            cout << "\nInvalid choice! Please enter a number.\n";

            continue;
        }

        switch (choice) {

            case 1:
                createAccount(accounts);
                break;

            case 2:
                depositMoney(accounts);
                break;

            case 3:
                withdrawMoney(accounts);
                break;

            case 4:
                checkBalance(accounts);
                break;

            case 5:
                accountDetails(accounts);
                break;

            case 6:
                transferMoney(accounts);
                break;

            case 7:
                transactionHistory();
                break;

            case 8:
                closeAccount(accounts);
                break;

            case 9:

                cout << "\nThank you for using Bank Management System!\n";
                cout << "Goodbye!\n";

                return 0;

            default:

                cout << "\nInvalid choice! Please choose 1-9.\n";
        }
    }

    return 0;
}
