#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class BookRecord {
private:
    string assetId;
    string bookTitle;
    string authorName;
    string statusFlag;

public:
    BookRecord() {
        assetId = "";
        bookTitle = "";
        authorName = "";
        statusFlag = "Available";
    }

    BookRecord(string id, string title, string author, string status) {
        assetId = id;
        bookTitle = title;
        authorName = author;
        statusFlag = status;
    }

    string getAssetId() const { return assetId; }
    string getBookTitle() const { return bookTitle; }
    string getAuthorName() const { return authorName; }
    string getStatusFlag() const { return statusFlag; }

    void updateStatus(string newStatus) {
        statusFlag = newStatus;
    }
};

void appendNewAsset();
void queryAllAssets();
void searchAssetDetails();
void processAssetRental(bool isReturnAction);
void displayLibraryMenu();

const string CATALOG_VAULT = "catalog.dat";

int main() {
    int controlInput;
    while (true) {
        displayLibraryMenu();
        cout << "Select an option (1-6): ";
        if (!(cin >> controlInput)) {
            cout << "Invalid entry! Please input a valid option.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (controlInput) {
            case 1: appendNewAsset(); break;
            case 2: queryAllAssets(); break;
            case 3: searchAssetDetails(); break;
            case 4: processAssetRental(false); break;
            case 5: processAssetRental(true); break;
            case 6: cout << "\nShutting down Library Portal. Goodbye!\n"; return 0;
            default: cout << "Selection out of bounds! Try again.\n";
        }
    }
    return 0;
}

void displayLibraryMenu() {
    cout << "\n=========================================\n";
    cout << "       LIBRARY MANAGEMENT SYSTEM         \n";
    cout << "=========================================\n";
    cout << "1. Add New Book Entry\n";
    cout << "2. View All Books in Catalog\n";
    cout << "3. Search for Book by Title/Author\n";
    cout << "4. Issue / Rent out a Book\n";
    cout << "5. Return a Borrowed Book\n";
    cout << "6. Exit Library Portal\n";
    cout << "-----------------------------------------\n";
}

void appendNewAsset() {
    ofstream appendStream(CATALOG_VAULT, ios::app);
    if (!appendStream) {
        cout << "Catalog storage error!\n";
        return;
    }

    string id, title, author;
    cin.ignore();

    cout << "\n--- Add New Book Entry ---\n";
    cout << "Enter unique Book ID: ";
    getline(cin, id);
    cout << "Enter Book Title    : ";
    getline(cin, title);
    cout << "Enter Author Name   : ";
    getline(cin, author);

    appendStream << id << "," << title << "," << author << ",Available\n";
    appendStream.close();

    cout << "\nBook cataloged successfully into storage.\n";
}

void queryAllAssets() {
    ifstream readStream(CATALOG_VAULT);
    if (!readStream) {
        cout << "\nNo catalog files discovered.\n";
        return;
    }

    string entryRow;
    cout << "\n=================================================================================\n";
    cout << left << setw(12) << "Book ID" << setw(30) << "Title" << setw(23) << "Author" << setw(12) << "Status" << "\n";
    cout << "=================================================================================\n";

    bool catalogIsEmpty = true;
    while (getline(readStream, entryRow)) {
        if (entryRow.empty()) continue;
        catalogIsEmpty = false;

        size_t break1 = entryRow.find(',');
        size_t break2 = entryRow.find(',', break1 + 1);
        size_t break3 = entryRow.find(',', break2 + 1);

        string id = entryRow.substr(0, break1);
        string title = entryRow.substr(break1 + 1, break2 - break1 - 1);
        string author = entryRow.substr(break2 + 1, break3 - break2 - 1);
        string status = entryRow.substr(break3 + 1);

        cout << left << setw(12) << id << setw(30) << title << setw(23) << author << setw(12) << status << "\n";
    }

    if (catalogIsEmpty) {
        cout << "The current library catalog is empty.\n";
    }
    cout << "=================================================================================\n";
    readStream.close();
}

void searchAssetDetails() {
    ifstream readStream(CATALOG_VAULT);
    if (!readStream) {
        cout << "\nCatalog file is missing.\n";
        return;
    }

    string searchKeyword;
    cout << "\nEnter Search Keyword (Title or Author): ";
    cin.ignore();
    getline(cin, searchKeyword);

    string lowerQuery = searchKeyword;
    for (char &c : lowerQuery) c = tolower(c);

    string entryRow;
    bool itemLocated = false;

    cout << "\n--- Matching Search Results ---\n";
    while (getline(readStream, entryRow)) {
        if (entryRow.empty()) continue;

        size_t break1 = entryRow.find(',');
        size_t break2 = entryRow.find(',', break1 + 1);
        size_t break3 = entryRow.find(',', break2 + 1);

        string id = entryRow.substr(0, break1);
        string title = entryRow.substr(break1 + 1, break2 - break1 - 1);
        string author = entryRow.substr(break2 + 1, break3 - break2 - 1);
        string status = entryRow.substr(break3 + 1);

        string lowerTitle = title; for (char &c : lowerTitle) c = tolower(c);
        string lowerAuthor = author; for (char &c : lowerAuthor) c = tolower(c);

        if (lowerTitle.find(lowerQuery) != string::npos || lowerAuthor.find(lowerQuery) != string::npos) {
            if (!itemLocated) {
                cout << left << setw(12) << "ID" << setw(30) << "Title" << setw(23) << "Author" << setw(12) << "Status" << "\n";
                cout << "---------------------------------------------------------------------------------\n";
            }
            itemLocated = true;
            cout << left << setw(12) << id << setw(30) << title << setw(23) << author << setw(12) << status << "\n";
        }
    }
    readStream.close();

    if (!itemLocated) {
        cout << "No matching entries discovered for: \"" << searchKeyword << "\"\n";
    }
}

void processAssetRental(bool isReturnAction) {
    ifstream readStream(CATALOG_VAULT);
    if (!readStream) {
        cout << "\nNo registered media catalog files found.\n";
        return;
    }

    string targetId;
    cout << "\nEnter the Book ID to process: ";
    cin.ignore();
    getline(cin, targetId);

    vector<BookRecord> internalCatalog;
    string entryRow;
    bool transactionProcessed = false;

    while (getline(readStream, entryRow)) {
        if (entryRow.empty()) continue;

        size_t break1 = entryRow.find(',');
        size_t break2 = entryRow.find(',', break1 + 1);
        size_t break3 = entryRow.find(',', break2 + 1);

        string id = entryRow.substr(0, break1);
        string title = entryRow.substr(break1 + 1, break2 - break1 - 1);
        string author = entryRow.substr(break2 + 1, break3 - break2 - 1);
        string status = entryRow.substr(break3 + 1);

        BookRecord currentBook(id, title, author, status);

        if (id == targetId) {
            if (isReturnAction) {
                if (status == "Issued") {
                    currentBook.updateStatus("Available");
                    transactionProcessed = true;
                    cout << "\nSuccess: \"" << title << "\" has been safely returned to collection.\n";
                } else {
                    cout << "\nAlert: This book is already marked as available in catalog records.\n";
                }
            } else {
                if (status == "Available") {
                    currentBook.updateStatus("Issued");
                    transactionProcessed = true;
                    cout << "\nSuccess: \"" << title << "\" has been securely checked out / issued.\n";
                } else {
                    cout << "\nAlert: This item is currently unavailable and checked out by a member.\n";
                }
            }
        }
        internalCatalog.push_back(currentBook);
    }
    readStream.close();

    if (!transactionProcessed) {
        return;
    }

    ofstream writeStream(CATALOG_VAULT, ios::trunc);
    for (const auto& bk : internalCatalog) {
        writeStream << bk.getAssetId() << "," << bk.getBookTitle() << "," << bk.getAuthorName() << "," << bk.getStatusFlag() << "\n";
    }
    writeStream.close();
}
