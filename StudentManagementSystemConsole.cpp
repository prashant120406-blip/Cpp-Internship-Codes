#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Student {
    string rollNumber;
    string name;
    string course;
    string age;
};

void addStudent();
void displayAllStudents();
void updateStudent();
void deleteStudent();
void showMenu();

const string FILE_NAME = "stu_data.txt";

int main() {
    int choice;
    while (true) {
        showMenu();
        cout << "Enter your choice (1-5): ";
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                updateStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                cout << "\nExiting the system.\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}

void showMenu() {
    cout << "\n=========================================\n";
    cout << "        STUDENT MANAGEMENT SYSTEM        \n";
    cout << "=========================================\n";
    cout << "1. Add New Student Record\n";
    cout << "2. Display All Student Records\n";
    cout << "3. Update Student Record\n";
    cout << "4. Delete Student Record\n";
    cout << "5. Exit System\n";
    cout << "-----------------------------------------\n";
}

void addStudent() {
    ofstream outFile(FILE_NAME, ios::app);
    if (!outFile) {
        cout << "Error opening file for writing!\n";
        return;
    }

    Student s;
    cin.ignore();

    cout << "\n--- Add New Student ---\n";
    cout << "Enter Roll Number: ";
    getline(cin, s.rollNumber);
    cout << "Enter Name       : ";
    getline(cin, s.name);
    cout << "Enter Course     : ";
    getline(cin, s.course);
    cout << "Enter Age        : ";
    getline(cin, s.age);

    outFile << s.rollNumber << "," << s.name << "," << s.course << "," << s.age << "\n";
    outFile.close();

    cout << "\nRecord added successfully!\n";
}

void displayAllStudents() {
    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "\nNo records found\n";
        return;
    }

    string line;
    cout << "\n===================================================================\n";
    cout << left << setw(15) << "Roll No" << setw(25) << "Name" << setw(15) << "Course" << setw(10) << "Age" << "\n";
    cout << "===================================================================\n";

    bool hasRecords = false;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        hasRecords = true;

        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        string roll = line.substr(0, pos1);
        string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string course = line.substr(pos2 + 1, pos3 - pos2 - 1);
        string age = line.substr(pos3 + 1);

        cout << left << setw(15) << roll << setw(25) << name << setw(15) << course << setw(10) << age << "\n";
    }

    if (!hasRecords) {
        cout << "No records available.\n";
    }
    cout << "===================================================================\n";
    inFile.close();
}

void updateStudent() {
    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "\nNo records found to update.\n";
        return;
    }

    string targetRoll;
    cout << "\nEnter the Roll Number to update: ";
    cin.ignore();
    getline(cin, targetRoll);

    vector<Student> students;
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        Student s;
        s.rollNumber = line.substr(0, pos1);
        s.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        s.course = line.substr(pos2 + 1, pos3 - pos2 - 1);
        s.age = line.substr(pos3 + 1);

        if (s.rollNumber == targetRoll) {
            found = true;
            cout << "\nRecord Found! Enter New Details:\n";
            cout << "Enter New Name  : ";
            getline(cin, s.name);
            cout << "Enter New Course: ";
            getline(cin, s.course);
            cout << "Enter New Age   : ";
            getline(cin, s.age);
        }
        students.push_back(s);
    }
    inFile.close();

    if (!found) {
        cout << "Student with Roll Number " << targetRoll << " not found.\n";
        return;
    }

    ofstream outFile(FILE_NAME, ios::trunc);
    for (const auto& s : students) {
        outFile << s.rollNumber << "," << s.name << "," << s.course << "," << s.age << "\n";
    }
    outFile.close();

    cout << "\nRecord updated successfully!\n";
}

void deleteStudent() {
    ifstream inFile(FILE_NAME);
    if (!inFile) {
        cout << "\nNo records found to delete.\n";
        return;
    }

    string targetRoll;
    cout << "\nEnter the Roll Number to delete: ";
    cin.ignore();
    getline(cin, targetRoll);

    vector<Student> students;
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        Student s;
        s.rollNumber = line.substr(0, pos1);
        s.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        s.course = line.substr(pos2 + 1, pos3 - pos2 - 1);
        s.age = line.substr(pos3 + 1);

        if (s.rollNumber == targetRoll) {
            found = true;
            continue;
        }
        students.push_back(s);
    }
    inFile.close();

    if (!found) {
        cout << "Student with Roll Number " << targetRoll << " not found.\n";
        return;
    }

    ofstream outFile(FILE_NAME, ios::trunc);
    for (const auto& s : students) {
        outFile << s.rollNumber << "," << s.name << "," << s.course << "," << s.age << "\n";
    }
    outFile.close();

    cout << "\nRecord deleted successfully!\n";
}
