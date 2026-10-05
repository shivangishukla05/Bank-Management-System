#include "crow_all.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>

using namespace std;

struct Account {
    long long accountNumber;
    string name;
    int age;
    string phone;
    string pin;
    double balance;
};

const string ACCOUNT_FILE = "accounts.txt";
const string TRANSACTION_FILE = "transactions.txt";


// ==================== CORS RESPONSE ====================

crow::response responseWithCORS(int code, const string& message)
{
    crow::response res(code, message);

    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");

    return res;
}


// ==================== TIME ====================

string getCurrentTime()
{
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

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

bool validPIN(const string& pin)
{
    if (pin.length() != 6)
        return false;

    for (char c : pin)
    {
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    }

    return true;
}


// ==================== LOAD ACCOUNTS ====================

vector<Account> loadAccounts()
{
    vector<Account> accounts;

    ifstream file(ACCOUNT_FILE);

    if (!file)
        return accounts;

    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        Account acc;

        string accountNumber;
        string age;
        string balance;

        getline(ss, accountNumber, '|');
        getline(ss, acc.name, '|');
        getline(ss, age, '|');
        getline(ss, acc.phone, '|');
        getline(ss, acc.pin, '|');
        getline(ss, balance, '|');

        try
        {
            acc.accountNumber = stoll(accountNumber);
            acc.age = stoi(age);
            acc.balance = stod(balance);

            accounts.push_back(acc);
        }
        catch (...)
        {
            continue;
        }
    }

    file.close();

    return accounts;
}


// ==================== SAVE ACCOUNTS ====================

void saveAccounts(const vector<Account>& accounts)
{
    ofstream file(ACCOUNT_FILE);

    for (const Account& acc : accounts)
    {
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

long long generateAccountNumber(const vector<Account>& accounts)
{
    long long number = 1001;

    for (const Account& acc : accounts)
    {
        if (acc.accountNumber >= number)
            number = acc.accountNumber + 1;
    }

    return number;
}


// ==================== FIND ACCOUNT ====================

int findAccount(
    const vector<Account>& accounts,
    long long accountNumber)
{
    for (int i = 0; i < static_cast<int>(accounts.size()); i++)
    {
        if (accounts[i].accountNumber == accountNumber)
            return i;
    }

    return -1;
}


// ==================== SAVE TRANSACTION ====================

void saveTransaction(
    long long accountNumber,
    const string& type,
    double amount,
    double balance)
{
    ofstream file(TRANSACTION_FILE, ios::app);

    file << accountNumber << "|"
         << getCurrentTime() << "|"
         << type << "|"
         << fixed << setprecision(2)
         << amount << "|"
         << balance << "\n";

    file.close();
}


// ==================== MAIN ====================

int main()
{
    crow::SimpleApp app;


    // ==================== HOME ====================

    CROW_ROUTE(app, "/")
    ([]()
    {
        return "Bank Management System Backend is Running!";
    });


    // ==================== CREATE ACCOUNT ====================

    CROW_ROUTE(app, "/create")
    .methods(crow::HTTPMethod::POST)
    ([&app](const crow::request& req)
    {
        auto body = crow::json::load(req.body);

        if (!body)
            return responseWithCORS(400, "Invalid request.");

        string name = body["name"].s();
        int age = body["age"].i();
        string phone = body["phone"].s();
        string
