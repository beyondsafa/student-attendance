#include "TinySHA1.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
using namespace std;

// Forward declaration
template<typename T>
void saveAllToFile(const string& filename, const unordered_map<string, T>& data);

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

    Person(string username, string hashedPassword, bool isHashed)
        : username(username), hashedPassword(isHashed ? hashedPassword : hashPassword(hashedPassword)) {}

    bool login(string inputUsername, string inputPassword) {
        return inputUsername == username && hashPassword(inputPassword) == hashedPassword;
    }

    string getUsername() const { return username; }
    string getHashedPassword() const { return hashedPassword; }
};

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
        attendance.clear();
        string line;
        while (getline(inFile, line)) {
            if (line == "END") break;
            istringstream iss(line);
            string date;
            int presentInt;
            if (iss >> date >> presentInt) {
                attendance[date] = (presentInt == 1);
            } else {
                cerr << "Invalid attendance entry: " << line << endl;
            }
        }
    }

    string getID() const { return studentID; }
};

void saveAllStudentsToFile(const string& filename, const unordered_map<string, Student>& students) {
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

unordered_map<string, Student> loadAllStudentsFromFile(const string& filename) {
    unordered_map<string, Student> students;
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            if (!line.empty()) {
                string id = line;
                Student student(id);
                student.loadFromFile(inFile);
                students[id] = student;
            }
        }
        inFile.close();
    }
    return students;
}

// Derived class: Teacher
class Teacher : public Person {
public:
    Teacher(string username = "", string password = "") : Person(username, password) {}
    Teacher(string username, string hashedPassword, bool isHashed) : Person(username, hashedPassword, isHashed) {}

    void addStudent(unordered_map<string, Student>& students) {
        while (true) {
            string studentID;
            cout << "Enter new Student ID (or type 'done' to finish): ";
            cin >> studentID;
            if (studentID == "done") break;
            if (students.find(studentID) == students.end()) {
                students[studentID] = Student(studentID);
                cout << "Student added successfully." << endl;
            } else {
                cout << "Student already exists." << endl;
            }
        }
        saveAllStudentsToFile("students.txt", students);
    }

    void takeAttendance(unordered_map<string, Student>& students) {
        cout << "Taking attendance..." << endl;
        string date;
        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> date;
        while (true) {
            string studentID;
            char status;
            cout << "Enter Student ID (or type 'done' to finish): ";
            cin >> studentID;
            if (studentID == "done") {
                cout << "Attendance marking completed." << endl;
                break;
            }
            cout << "Is the student present? (y/n): ";
            cin >> status;
            if (status != 'y' && status != 'Y' && status != 'n' && status != 'N') {
                cout << "Invalid input." << endl;
                continue;
            }
            auto it = students.find(studentID);
            if (it != students.end()) {
                it->second.markAttendance(date, (status == 'y' || status == 'Y'));
            } else {
                cout << "Student not found!" << endl;
            }
        }
        saveAllStudentsToFile("students.txt", students);
    }

    void viewAttendanceReport(const unordered_map<string, Student>& students) const {
        cout << "Attendance Report:" << endl;
        for (const auto& [id, student] : students) {
            student.viewAttendance();
        }
    }

    void menu(unordered_map<string, Student>& students) {
        char choice;
        do {
            cout << "\nTeacher Menu\n1. Take Attendance\n2. View Attendance Report\n3. Add Student\n4. Logout\n";
            cin >> choice;
            switch (choice) {
            case '1': takeAttendance(students); break;
            case '2': viewAttendanceReport(students); break;
            case '3': addStudent(students); break;
            case '4': cout << "Logging out..." << endl; break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != '4');
    }
};

unordered_map<string, Teacher> loadAllTeachersFromFile(const string& filename) {
    unordered_map<string, Teacher> teachers;
    ifstream inFile(filename);
    string username, hashedPass;
    while (inFile >> username >> hashedPass) {
        teachers[username] = Teacher(username, hashedPass, true);
    }
    return teachers;
}

void saveAllTeachersToFile(const string& filename, const unordered_map<string, Teacher>& teachers) {
    ofstream outFile(filename);
    for (const auto& [username, teacher] : teachers) {
        outFile << teacher.getUsername() << " " << teacher.getHashedPassword() << endl;
    }
}

// Derived class: Admin
class Admin : public Person {
private:
    unordered_map<string, Student> students;
    unordered_map<string, Teacher> teachers;

public:
    Admin(string username, string password)
        : Person(username, password) {
        students = loadAllStudentsFromFile("students.txt");
        teachers = loadAllTeachersFromFile("teachers.txt");
    }

    void menu() {
        char choice;
        do {
            cout << "\nAdmin Menu\n1. Remove Student\n2. Add Teacher\n3. Load Data\n4. Logout\n";
            cin >> choice;
            switch (choice) {
            case '1': removeStudent(); break;
            case '2': addTeacher(); break;
            case '3': loadData(); break;
            case '4': cout << "Logging out..." << endl; break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != '4');
    }

    void removeStudent() {
        string studentID;
        cout << "Enter Student ID to remove: ";
        cin >> studentID;
        if (students.erase(studentID)) {
            cout << "Student removed." << endl;
            saveAllStudentsToFile("students.txt", students);
        } else {
            cout << "Not found." << endl;
        }
    }

    void addTeacher() {
        string tUser, tPass;
        cout << "Enter Teacher Username: "; cin >> tUser;
        cout << "Enter Teacher Password: "; cin >> tPass;
        Teacher newTeacher(tUser, tPass);
        teachers[tUser] = newTeacher;
        saveAllTeachersToFile("teachers.txt", teachers);
        cout << "Teacher added!" << endl;
    }

    void loadData() {
        students = loadAllStudentsFromFile("students.txt");
        cout << "Students loaded." << endl;
    }
};

void mainMenu() {
    char choice;
    do {
        cout << "\nWelcome to Attendance System\n1. Login as Admin\n2. Login as Teacher\n3. View Attendance as Student\n4. Exit\n";
        cin >> choice;
        switch (choice) {
        case '1': {
            Admin admin("admin", "admin");
            string username, password;
            cout << "Enter Admin Username: "; cin >> username;
            cout << "Enter Admin Password: "; cin >> password;
            if (admin.login(username, password)) admin.menu();
            else cout << "Invalid credentials!" << endl;
            break;
        }
        case '2': {
            string username, password;
            cout << "Enter Teacher Username: "; cin >> username;
            cout << "Enter Teacher Password: "; cin >> password;
            auto teachers = loadAllTeachersFromFile("teachers.txt");
            auto it = teachers.find(username);
            if (it != teachers.end() && it->second.login(username, password)) {
                auto students = loadAllStudentsFromFile("students.txt");
                it->second.menu(students);
                saveAllStudentsToFile("students.txt", students);
            } else {
                cout << "Invalid credentials!" << endl;
            }
            break;
        }
        case '3': {
            string studentID;
            cout << "Enter Student ID: ";
            cin >> studentID;
            auto students = loadAllStudentsFromFile("students.txt");
            auto it = students.find(studentID);
            if (it != students.end()) {
                it->second.viewAttendance();
            } else {
                cout << "Student not found." << endl;
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
