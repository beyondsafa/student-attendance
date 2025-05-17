#include "TinySHA1.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map> // For storing attendance records
#include <fstream>       // For file I/O
#include <sstream>       // For string stream
using namespace std;

// Forward declaration of Teacher class
class Teacher;

// Standalone class: Student
class Student {
private:
    string studentID;
    unordered_map<string, bool> attendance; // Store attendance (true = present)

public:
    Student(string id) : studentID(id) {}

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
            outFile << "END" << endl; // Mark the end of this student's data
        }
    }

    void loadFromFile(ifstream& inFile) {
        string line;
        while (getline(inFile, line)) {
            if (line == studentID) {
                while (getline(inFile, line) && line != "END") {
                    string date;
                    bool isPresent;
                    istringstream iss(line);
                    iss >> date >> isPresent;
                    attendance[date] = isPresent;
                }
                break;
            }
        }
    }
};

// Base class: Person
class Person {
protected:
    string username;
    string hashedPassword;

    string hashPassword(const string& password) {
        sha1::SHA1 sha1;
        sha1.processBytes(password.data(), password.size());
    
        // Use getDigestBytes to retrieve the hash as bytes
        sha1::SHA1::digest8_t digest;
        sha1.getDigestBytes(digest);
    
        // Convert the digest to a hexadecimal string
        stringstream ss;
        for (int i = 0; i < 20; ++i) {
            ss << hex << setw(2) << setfill('0') << static_cast<int>(digest[i]);
        }
        return ss.str();
    }

public:
    Person(string username = "", string password = "")
        : username(username), hashedPassword(hashPassword(password)) {}

    void login(string inputUsername, string inputPassword) {
        if (inputUsername == username && hashPassword(inputPassword) == hashedPassword) {
            cout << "Login Successful!" << endl;
        } else {
            cout << "Invalid Credentials!" << endl;
        }
    }
};

// Derived class: Admin
class Admin : public Person {
private:
    Teacher* teacher; // Pointer to a single Teacher object
    unordered_map<string, Student> students; // Store students by ID

public:
    Admin(string username, string password)
        : Person(username, password), teacher(nullptr) {}

    ~Admin() {
        if (teacher != nullptr) {
            delete teacher; // Clean up dynamically allocated memory
        }
    }

    void menu() {
        cout << "Admin Menu" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Remove Student" << endl;
        cout << "3. Add Teacher" << endl;
        cout << "4. View Attendance Report" << endl;
        cout << "5. Save Data to File" << endl;
        cout << "6. Load Data from File" << endl;
        cout << "7. Logout" << endl;
        char choice;
        cin >> choice;
        switch (choice) {
        case '1':
            addStudent();
            break;
        case '2':
            removeStudent();
            break;
        case '3':
            addTeacher();
            break;
        case '4':
            viewAttendanceReport();
            break;
        case '5':
            saveAllStudentsToFile("students.txt");
            break;
        case '6':
            loadAllStudentsFromFile("students.txt");
            break;
        case '7':
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    }

    void addStudent() {
        cout << "Adding a student..." << endl;
        string studentID;
        cout << "Enter Student ID to add: ";
        cin >> studentID;
        students[studentID] = Student(studentID);
        cout << "Student with ID " << studentID << " added successfully!" << endl;
    }

    void removeStudent() {
        cout << "Removing a student..." << endl;
        string studentID;
        cout << "Enter Student ID to remove: ";
        cin >> studentID;
        if (students.erase(studentID)) {
            cout << "Student with ID " << studentID << " removed successfully!" << endl;
        } else {
            cout << "Student with ID " << studentID << " not found!" << endl;
        }
    }

    void addTeacher() {
        if (teacher != nullptr) {
            cout << "A teacher already exists. Overwriting..." << endl;
            delete teacher; // Delete the existing teacher
        }
        string teacherUsername, teacherPassword;
        cout << "Enter Teacher Username: ";
        cin >> teacherUsername;
        cout << "Enter Teacher Password: ";
        cin >> teacherPassword;
        teacher = new Teacher(teacherUsername, teacherPassword);
        cout << "Teacher added successfully!" << endl;
    }

    void viewAttendanceReport() {
        cout << "Viewing attendance report..." << endl;
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
            cout << "All student data saved successfully!" << endl;
        } else {
            cerr << "Error: Unable to open file for saving!" << endl;
        }
    }

    void loadAllStudentsFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                if (!line.empty() && line != "END") {
                    string studentID = line;
                    Student student(studentID);
                    student.loadFromFile(inFile);
                    students[studentID] = student;
                }
            }
            inFile.close();
            cout << "All student data loaded successfully!" << endl;
        } else {
            cerr << "Error: Unable to open file for loading!" << endl;
        }
    }
};

// Derived class: Teacher
class Teacher : public Person {
private:
    unordered_map<string, bool> attendance; // Store attendance (true = present)

public:
    Teacher(string username, string password)
        : Person(username, password) {}

    void menu() {
        cout << "Teacher Menu" << endl;
        cout << "1. Mark Attendance" << endl;
        cout << "2. View Attendance Report" << endl;
        cout << "3. Logout" << endl;
        char choice;
        cin >> choice;
        switch (choice) {
        case '1':
            markAttendance();
            break;
        case '2':
            viewAttendanceReport();
            break;
        case '3':
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    }

    void markAttendance() {
        cout << "Marking attendance..." << endl;
        string studentID, date;
        char status;
        cout << "Enter Student ID: ";
        cin >> studentID;
        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> date;
        cout << "Is the student present? (y/n): ";
        cin >> status;
        attendance[studentID] = (status == 'y' || status == 'Y');
        cout << "Attendance marked for Student ID: " << studentID << " on " << date << endl;
    }

    void viewAttendanceReport() {
        cout << "Viewing attendance report..." << endl;
        for (const auto& [id, isPresent] : attendance) {
            cout << "Student ID: " << id << " - " << (isPresent ? "Present" : "Absent") << endl;
        }
    }
};

// Main menu function
void mainMenu() {
    cout << "Welcome to Attendance System" << endl;
    cout << "1. Login as Admin" << endl;
    cout << "2. Login as Teacher" << endl;
    cout << "3. View Attendance as Student" << endl;
    cout << "4. Exit" << endl;
    char choice;
    cin >> choice;

    switch (choice) {
    case '1': {
        Admin admin("admin", "admin");
        string username, password;
        cout << "Enter Admin Username: ";
        cin >> username;
        cout << "Enter Admin Password: ";
        cin >> password;
        admin.login(username, password);
        admin.menu();
        break;
    }
    case '2': {
        Teacher teacher("teacher", "teacher");
        string username, password;
        cout << "Enter Teacher Username: ";
        cin >> username;
        cout << "Enter Teacher Password: ";
        cin >> password;
        teacher.login(username, password);
        teacher.menu();
        break;
    }
    case '3': {
        string studentID;
        cout << "Enter Student ID: ";
        cin >> studentID;
        Student student(studentID);
        student.viewAttendance();
        break;
    }
    case '4':
        cout << "Exiting..." << endl;
        break;
    default:
        cout << "Invalid choice!" << endl;
    }
}

int main() {
    mainMenu();
    return 0;
}