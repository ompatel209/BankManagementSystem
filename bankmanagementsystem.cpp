#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>  // Missing include for vector
using namespace std;

class BankAccount {
private:
    string accountNumber;
    string name;
    string type;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cin.ignore();
        cout << "Enter Account Holder Name: ";
        getline(cin, name);
        cout << "Enter Account Type (Savings/Current): ";
        cin >> type;
        cout << "Enter Initial Balance: ";
        cin >> balance;
    }

    void showAccount() const {
        cout << left << setw(15) << accountNumber
            << setw(25) << name
            << setw(15) << type
            << "$" << balance << endl;
    }

    string getAccountNumber() const { return accountNumber; }

    void deposit(double amount) { balance += amount; }
    void withdraw(double amount) {
        if (amount > balance)
            cout << "\nInsufficient balance!" << endl;
        else
            balance -= amount;
    }

    void saveToFile(ofstream& out) const {
        out << accountNumber << "," << name << "," << type << "," << balance << endl;
    }

    void loadFromCSV(const string& line) {
        size_t pos = 0, start = 0;
        int field = 0;
        string temp;

        while ((pos = line.find(',', start)) != string::npos && field < 3) {
            temp = line.substr(start, pos - start);
            if (field == 0) accountNumber = temp;
            else if (field == 1) name = temp;
            else if (field == 2) type = temp;
            start = pos + 1;
            field++;
        }
        balance = stod(line.substr(start));
    }
};

void displayMenu() {
    cout << "\n===== Bank Management System =====" << endl;
    cout << "1. Create Account" << endl;
    cout << "2. Display All Accounts" << endl;
    cout << "3. Deposit" << endl;
    cout << "4. Withdraw" << endl;
    cout << "5. Exit" << endl;
    cout << "Choose an option: ";
}

void saveAll(const vector<BankAccount>& accounts) {
    ofstream file("accounts.txt");
    for (const auto& acc : accounts)
        acc.saveToFile(file);
    file.close();
}

void loadAll(vector<BankAccount>& accounts) {
    accounts.clear();
    ifstream file("accounts.txt");
    string line;
    while (getline(file, line)) {
        BankAccount acc;
        acc.loadFromCSV(line);
        accounts.push_back(acc);
    }
    file.close();
}

int main() {
    vector<BankAccount> accounts;
    loadAll(accounts);
    int choice;
    string accNum;
    double amount;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            BankAccount acc;
            acc.createAccount();
            accounts.push_back(acc);
            cout << "Account created successfully!" << endl;
            break;
        }
        case 2:
            cout << left << setw(15) << "Account No"
                << setw(25) << "Name"
                << setw(15) << "Type"
                << "Balance" << endl;
            for (const auto& acc : accounts)
                acc.showAccount();
            break;
        case 3:
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Deposit Amount: ";
            cin >> amount;
            for (auto& acc : accounts) {
                if (acc.getAccountNumber() == accNum) {
                    acc.deposit(amount);
                    cout << "Deposit successful." << endl;
                    break;
                }
            }
            break;
        case 4:
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Withdrawal Amount: ";
            cin >> amount;
            for (auto& acc : accounts) {
                if (acc.getAccountNumber() == accNum) {
                    acc.withdraw(amount);
                    cout << "Transaction processed." << endl;
                    break;
                }
            }
            break;
        case 5:
            saveAll(accounts);
            cout << "Goodbye! Data saved." << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 5);

    return 0;
}