#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
class UserProfile {
private:
    string referenceId;
    string clientName;
    double walletFunds;

public:
    UserProfile() {
        referenceId = "";
        clientName = "";
        walletFunds = 0.0;
    }

    UserProfile(string id, string cName, double funds) {
        referenceId = id;
        clientName = cName;
        walletFunds = funds;
    }

    string getRefId() const { return referenceId; }
    string getClientName() const { return clientName; }
    double getFunds() const { return walletFunds; }

    void addFunds(double money) {
        if (money > 0) {
            walletFunds += money;
            cout << "\nSuccess: Added ₹" << money << " to the profile.";
        } else {
            cout << "\nError: Invalid amount entered.";
        }
    }

    bool removeFunds(double money) {
        if (money > 0 && money <= walletFunds) {
            walletFunds -= money;
            cout << "\nSuccess: Deducted ₹" << money << " from the profile.";
            return true;
        } else {
            cout << "\nError: Insufficient funds or invalid amount.";
            return false;
        }
    }
};

void insertNewProfile();
void processCredit();
void processDebit();
void viewBalanceSheet();
void showNavigationMenu();

const string DATA_VAULT = "ledger.dat";

int main() {
    int selection;
    while (true) {
        showNavigationMenu();
        cout << "Select an option (1-5): ";
        if (!(cin >> selection)) {
            cout << "Invalid entry! Please input a valid option.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (selection) {
            case 1: insertNewProfile(); break;
            case 2: processCredit(); break;
            case 3: processDebit(); break;
            case 4: viewBalanceSheet(); break;
            case 5: cout << "\nClosing Banking System. Goodbye!\n"; return 0;
            default: cout << "Selection out of range! Try again.\n";
        }
    }
    return 0;
}

void showNavigationMenu() {
    cout << "\n=========================================\n";
    cout << "        BANK MANAGEMENT SYSTEM           \n";
    cout << "=========================================\n";
    cout << "1. New Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. Check Balance Inquiry\n";
    cout << "5. Exit Portal\n";
    cout << "-----------------------------------------\n";
}

void insertNewProfile() {
    ofstream appendStream(DATA_VAULT, ios::app);
    if (!appendStream) {
        cout << "Storage system error!\n";
        return;
    }

    string id, name;
    double initialSum;
    cin.ignore();

    cout << "\n--- Open New Account ---\n";
    cout << "Enter Account Number: ";
    getline(cin, id);
    cout << "Enter Account Holder Name: ";
    getline(cin, name);
    cout << "Enter Initial Balance: ₹";
    cin >> initialSum;

    appendStream << id << "," << name << "," << initialSum << "\n";
    appendStream.close();

    cout << "\nProfile registered successfully in local database.\n";
}

void processCredit() {
    ifstream readStream(DATA_VAULT);
    if (!readStream) {
        cout << "\nDatabase file missing.\n";
        return;
    }

    string searchId;
    double creditAmount;
    cout << "\nEnter Account Number for deposit: ";
    cin.ignore();
    getline(cin, searchId);

    vector<UserProfile> profileList;
    string row;
    bool isFound = false;

    while (getline(readStream, row)) {
        if (row.empty()) continue;

        size_t split1 = row.find(',');
        size_t split2 = row.find(',', split1 + 1);

        string parsedId = row.substr(0, split1);
        string parsedName = row.substr(split1 + 1, split2 - split1 - 1);
        double parsedFunds = stod(row.substr(split2 + 1));

        UserProfile user(parsedId, parsedName, parsedFunds);
        if (parsedId == searchId) {
            isFound = true;
            cout << "Verified User: " << parsedName << "\n";
            cout << "Enter amount to deposit: ₹";
            cin >> creditAmount;
            user.addFunds(creditAmount);
        }
        profileList.push_back(user);
    }
    readStream.close();

    if (!isFound) {
        cout << "Target account could not be located.\n";
        return;
    }

    ofstream writeStream(DATA_VAULT, ios::trunc);
    for (const auto& u : profileList) {
        writeStream << u.getRefId() << "," << u.getClientName() << "," << u.getFunds() << "\n";
    }
    writeStream.close();
}

void processDebit() {
    ifstream readStream(DATA_VAULT);
    if (!readStream) {
        cout << "\nFile missing.\n";
        return;
    }

    string searchId;
    double debitAmount;
    cout << "\nEnter Account Number for withdrawal: ";
    cin.ignore();
    getline(cin, searchId);

    vector<UserProfile> profileList;
    string row;
    bool isFound = false;

    while (getline(readStream, row)) {
        if (row.empty()) continue;

        size_t split1 = row.find(',');
        size_t split2 = row.find(',', split1 + 1);

        string parsedId = row.substr(0, split1);
        string parsedName = row.substr(split1 + 1, split2 - split1 - 1);
        double parsedFunds = stod(row.substr(split2 + 1));

        UserProfile user(parsedId, parsedName, parsedFunds);
        if (parsedId == searchId) {
            isFound = true;
            cout << "Verified User: " << parsedName << "\n";
            cout << "Available Balance: ₹" << parsedFunds << "\n";
            cout << "Enter amount to withdraw: ₹";
            cin >> debitAmount;
            user.removeFunds(debitAmount);
        }
        profileList.push_back(user);
    }
    readStream.close();

    if (!isFound) {
        cout << "Target account reference could not be located.\n";
        return;
    }

    ofstream writeStream(DATA_VAULT, ios::trunc);
    for (const auto& u : profileList) {
        writeStream << u.getRefId() << "," << u.getClientName() << "," << u.getFunds() << "\n";
    }
    writeStream.close();
}

void viewBalanceSheet() {
    ifstream readStream(DATA_VAULT);
    if (!readStream) {
        cout << "\nNo registered profiles active.\n";
        return;
    }

    string searchId;
    cout << "\nEnter Account Number: ";
    cin.ignore();
    getline(cin, searchId);

    string row;
    bool isFound = false;

    while (getline(readStream, row)) {
        if (row.empty()) continue;

        size_t split1 = row.find(',');
        size_t split2 = row.find(',', split1 + 1);

        string parsedId = row.substr(0, split1);
        string parsedName = row.substr(split1 + 1, split2 - split1 - 1);
        double parsedFunds = stod(row.substr(split2 + 1));

        if (parsedId == searchId) {
            isFound = true;
            cout << "\n=========================================";
            cout << "\n         ACCOUNT STATEMENT BALANCE       ";
            cout << "\n=========================================";
            cout << "\nAccount Number : " << parsedId;
            cout << "\nHolder Name    : " << parsedName;
            cout << "\nLedger Balance : ₹" << fixed << setprecision(2) << parsedFunds;
            cout << "\n=========================================\n";
            break;
        }
    }
    readStream.close();

    if (!isFound) {
        cout << "No matching records.\n";
    }
}
