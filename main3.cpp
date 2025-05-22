#include "TinySHA1.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
using namespace std;

// Base class: Person
class Person {
protected:
    string username;
    string hashedPassword;

    string hashPassword(const string& password) {
        sha1::SHA1 sha1;
        sha1.processBytes(password.data(), password.size());
        sha1::SHA1::digest8_t digest;
        sha1.getDigestBytes(digest);
        stringstream ss;
        for (int i = 0; i < 20; ++i) {
            ss << hex << setw(2) << setfill('0') << static_cast<int>(digest[i]);
        }
        return ss.str();
    }

public:
    Person(string username = "", string password = "")
        : username(username), hashedPassword(hashPassword(password)) {}

    bool login(string inputUsername, string inputPassword) {
        return inputUsername == username && hashPassword(inputPassword) == hashedPassword;
    }
};

// Forward declaration
class Student;

// Standalone class: Student
class Student {
private:
    string studentID;
    unordered_map<string, bool> attendance;

public:
    Student(string id = "") : studentID(id) {}

    void markAttendance(const string& date, bool isPresent) {
        attendance[date] = isPresent;
    }

    void viewAttendance() const {
        cout << "Attendance for Student ID: " << studentID << endl;
        for (const auto& [date, isPresent] : attendance) {
            cout << "Date: " << date << " - " << (isPresent ? "Present" : "Absent") << endl;
        }
    }

    void saveToFile(ofstream& outFile) const {
        if (outFile.is_open()) {
            outFile << studentID << endl;
            for (const auto& [date, isPresent] : attendance) {
                outFile << date << " " << isPresent << endl;
            }
            outFile << "END" << endl;
        }
    }

    void loadFromFile(ifstream& inFile) {
        string line;
        while (getline(inFile, line) && line != "END") {
            string date;
            bool isPresent;
            istringstream iss(line);
            iss >> date >> isPresent;
            attendance[date] = isPresent;
        }
    }
};

// Derived class: Teacher
class Teacher : public Person {
public:
    Teacher(string username, string password)
        : Person(username, password) {}

    void markAttendance(unordered_map<string, Student>& students) {
        cout << "Marking attendance..." << endl;
        string studentID, date;
        char status;
        cout << "Enter Student ID: ";
        cin >> studentID;
        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> date;
        cout << "Is the student present? (y/n): ";
        cin >> status;

        auto it = students.find(studentID);
        if (it != students.end()) {
            it->second.markAttendance(date, (status == 'y' || status == 'Y'));
            cout << "Attendance marked for Student ID: " << studentID << endl;
        } else {
            cout << "Student not found!" << endl;
        }
    }

    void viewAttendanceReport(const unordered_map<string, Student>& students) const {
        cout << "Viewing attendance report..." << endl;
        for (const auto& [id, student] : students) {
            student.viewAttendance();
        }
    }

    void menu(unordered_map<string, Student>& students) {
        char choice;
        do {
            cout << "\nTeacher Menu" << endl;
            cout << "1. Mark Attendance" << endl;
            cout << "2. View Attendance Report" << endl;
            cout << "3. Logout" << endl;
            cin >> choice;
            switch (choice) {
            case '1':
                markAttendance(students);
                break;
            case '2':
                viewAttendanceReport(students);
                break;
            case '3':
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
            }
        } while (choice != '3');
    }
};

// Derived class: Admin
class Admin : public Person {
private:
    Teacher* teacher;
    unordered_map<string, Student> students;

public:
    Admin(string username, string password)
        : Person(username="admin", password="admin"), teacher(nullptr) {}

    ~Admin() {
        delete teacher;
    }

    void menu() {
        char choice;
        do {
            cout << "\nAdmin Menu" << endl;
            cout << "1. Add Student" << endl;
            cout << "2. Remove Student" << endl;
            cout << "3. Add Teacher" << endl;
            cout << "4. View Attendance Report" << endl;
            cout << "5. Save Data to File" << endl;
            cout << "6. Load Data from File" << endl;
            cout << "7. Logout" << endl;
            cin >> choice;
            switch (choice) {
            case '1': addStudent(); break;
            case '2': removeStudent(); break;
            case '3': addTeacher(); break;
            case '4': viewAttendanceReport(); break;
            case '5': saveAllStudentsToFile("students.txt"); break;
            case '6': loadAllStudentsFromFile("students.txt"); break;
            case '7': cout << "Logging out..." << endl; break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != '7');
    }

    void addStudent() {
        string studentID;
        cout << "Enter Student ID to add: ";
        cin >> studentID;
        students[studentID] = Student(studentID);
        cout << "Student added successfully!" << endl;
    }

    void removeStudent() {
        string studentID;
        cout << "Enter Student ID to remove: ";
        cin >> studentID;
        if (students.erase(studentID)) {
            cout << "Student removed successfully!" << endl;
        } else {
            cout << "Student not found!" << endl;
        }
    }

    void addTeacher() {
        delete teacher;
        string tUser, tPass;
        cout << "Enter Teacher Username: "; cin >> tUser;
        cout << "Enter Teacher Password: "; cin >> tPass;
        teacher = new Teacher(tUser, tPass);
        cout << "Teacher added successfully!" << endl;
        teacher->menu(students);
    }

    void viewAttendanceReport() const {
        for (const auto& [id, student] : students) {
            student.viewAttendance();
        }
    }

    void saveAllStudentsToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& [id, student] : students) {
                student.saveToFile(outFile);
            }
            outFile.close();
            cout << "Data saved successfully!" << endl;
        } else {
            cerr << "Error opening file for saving!" << endl;
        }
    }

    void loadAllStudentsFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                if (!line.empty()) {
                    string studentID = line;
                    Student student(studentID);
                    student.loadFromFile(inFile);
                    students[studentID] = student;
                }
            }
            inFile.close();
            cout << "Data loaded successfully!" << endl;
        } else {
            cerr << "Error opening file for loading!" << endl;
        }
    }
};

void mainMenu() {
    char choice;
    do {
        cout << "\nWelcome to Attendance System" << endl;
        cout << "1. Login as Admin" << endl;
        cout << "2. Login as Teacher" << endl;
        cout << "3. View Attendance as Student" << endl;
        cout << "4. Exit" << endl;
        cin >> choice;

        switch (choice) {
        case '1': {
            Admin admin("admin", "admin");
            string username, password;
            cout << "Enter Admin Username: "; cin >> username;
            cout << "Enter Admin Password: "; cin >> password;
            if (admin.login(username, password)) {
                admin.menu();
            } else {
                cout << "Invalid credentials!" << endl;
            }
            break;
        }
        case '2': {
            cout << "Only Admin can create Teachers. Please login as Admin." << endl;
            break;
        }
        case '3': {
            string studentID;
            cout << "Enter Student ID: ";
            cin >> studentID;
            Student student(studentID);
            ifstream inFile("students.txt");
            if (inFile.is_open()) {
                string line;
                bool found = false;
                while (getline(inFile, line)) {
                    if (line == studentID) {
                        student.loadFromFile(inFile);
                        student.viewAttendance();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Student not found in records." << endl;
                }
                inFile.close();
            } else {
                cout << "Unable to open student file." << endl;
            }
            break;
        }
        case '4':
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != '4');
}

int main() {
    mainMenu();
    return 0;
}
