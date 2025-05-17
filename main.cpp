#include<iostream>
using namespace std;
class person
{ 
    private:
        string name;
};
int main()
{
   
    return 0;
}
void mainmenu()
{
    cout<<"Welcome to Attendance System"<<endl;
    cout<<"1. Login as Admin"<<endl;
    cout<<"2. Teacher Login"<<endl;
    cout<<"3. View Attendance Report"<<endl;
    cout<<"4. Exit"<<endl;
    char choice;
    cin>>choice;
    switch(choice)
    {
        case '1':
        //call Admin object member function
            break;
        case '2':
        //Call teacher object member function
            break;
            break;
        case '3':
    //call attendance_view function
         attendance_view();
             break;
        case '4':
    cout<<"Exititing"<<endl;
    break;
        default:
            cout<<"Invalid choice"<<endl;
    }
}
void attendance_view()
{
//show attendance report
cout<<"Attendance Report"<<endl;
cout<<"1. View by Student ID"<<endl;
cout<<"2. View by Class"<<endl;
cout<<"3. View by Date"<<endl;
cout<<"4. Back to Main Menu"<<endl;
char choice1;
cin>>choice1;
switch(choice1)
{
    case '1':
        //view by student
        break;
        case '2':
        //view by class
    break; 
    case '3':
        //view by date  
    break; 
    case '4':
        //back to main menu
        break;
    default:
        cout<<"Invalid choice"<<endl;
}
}
void admin_login()
{
    //admin login function
    string username;
    string password;
    cout<<"Enter username: ";
    cin>>username;
    cout<<"Enter Password: ";
    cin>>password;
    if(username=="admin" && password=="admin")
    {
        cout<<"Login Successful"<<endl;
        //call admin menu function
        admin_menu();
    }
    else
    {
        cout<<"Invalid username or password"<<endl;
        mainmenu();
    }
}
void admin_menu()
{
    cout<<"Admin Menu"<<endl;
    cout<<"1. Add Student"<<endl;
    cout<<"2. Remove Student"<<endl;
    cout<<"3. View Attendance Report"<<endl;
    cout<<"4. Logout"<<endl;
    char choice;
    cin>>choice;
    switch(choice)
    {
        case '1':
            //add student function
            break;
        case '2':
            //remove student function
            break;
        case '3':
            //view attendance report function
            break;
        case '4':
            //logout function
            break;
        default:
            cout<<"Invalid choice"<<endl;
    }
}
void teacher_login()
{
    //teacher login function
    string username;
    string password;
    cout<<"Enter username: ";
    cin>>username;
    cout<<"Enter Password: ";
    cin>>password;
    if(username=="teacher" && password=="teacher")
    {
        cout<<"Login Successful"<<endl;
        //call teacher menu function
        teacher_menu();
    }
    else
    {
        cout<<"Invalid username or password"<<endl;
        mainmenu();
    }
}
void teacher_menu()
{
    cout<<"Teacher Menu"<<endl;
    cout<<"1. Mark Attendance"<<endl;
    cout<<"2. View Attendance Report"<<endl;
    cout<<"3. Logout"<<endl;
    char choice;
    cin>>choice;
    switch(choice)
    {
        case '1':
            //mark attendance function
            break;
        case '2':
            //view attendance report function
            break;
        case '3':
            //logout function
            break;
        default:
            cout<<"Invalid choice"<<endl;
    }
}
void mark_attendance()
{
    //mark attendance function
    cout<<"Mark Attendance"<<endl;
    cout<<"Enter Student ID: ";
    string student_id;
    cin>>student_id;
    cout<<"Enter Date: ";
    string date;
    cin>>date;
    cout<<"Attendance marked for student "<<student_id<<" on "<<date<<endl;
}
void view_attendance_report()
{
    //view attendance report function
    cout<<"View Attendance Report"<<endl;
    cout<<"Enter Student ID: ";
    string student_id;
    cin>>student_id;
    cout<<"Attendance report for student "<<student_id<<endl;
}
void add_student()
{
    //add student function
    cout<<"Add Student"<<endl;
    cout<<"Enter Student ID: ";
    string student_id;
    cin>>student_id;
    cout<<"Enter Student Name: ";
    string student_name;
    cin>>student_name;
    cout<<"Student "<<student_name<<" with ID "<<student_id<<" added successfully"<<endl;
}
void remove_student()
{
    //remove student function
    cout<<"Remove Student"<<endl;
    cout<<"Enter Student ID: ";
    string student_id;
    cin>>student_id;
    cout<<"Student with ID "<<student_id<<" removed successfully"<<endl;
}
void logout()
{
    //logout function
    cout<<"Logout Successful"<<endl;
    mainmenu();
}
void view_attendance_by_student()
{
    //view attendance by student function
    cout<<"View Attendance by Student"<<endl;
    cout<<"Enter Student ID: ";
    string student_id;
    cin>>student_id;
    cout<<"Attendance report for student "<<student_id<<endl;
}
void view_attendance_by_class()
{
    //view attendance by class function
    cout<<"View Attendance by Class"<<endl;
    cout<<"Enter Class: ";
    string class_name;
    cin>>class_name;
    cout<<"Attendance report for class "<<class_name<<endl;
}
void view_attendance_by_date()
{
    //view attendance by date function
    cout<<"View Attendance by Date"<<endl;
    cout<<"Enter Date: ";
    string date;
    cin>>date;
    cout<<"Attendance report for date "<<date<<endl;
}
void view_attendance_by_student_id()
{
    //view attendance by student id function
    cout<<"View Attendance by Student ID"<<endl;
    cout<<"Enter Student ID: ";
    string student_id;
    cin>>student_id;
    cout<<"Attendance report for student "<<student_id<<endl;
}
void view_attendance_by_class_name()
{
    //view attendance by class name function
    cout<<"View Attendance by Class Name"<<endl;
    cout<<"Enter Class Name: ";
    string class_name;
    cin>>class_name;
    cout<<"Attendance report for class "<<class_name<<endl;
}
void view_attendance_by_date_range()
{
    //view attendance by date range function
    cout<<"View Attendance by Date Range"<<endl;
    cout<<"Enter Start Date: ";
    string start_date;
    cin>>start_date;
    cout<<"Enter End Date: ";
    string end_date;
    cin>>end_date;
    cout<<"Attendance report from "<<start_date<<" to "<<end_date<<endl;
}
void view_attendance_by_student_name()
{
    //view attendance by student name function
    cout<<"View Attendance by Student Name"<<endl;
    cout<<"Enter Student Name: ";
    string student_name;
    cin>>student_name;
    cout<<"Attendance report for student "<<student_name<<endl;
}
void view_attendance_by_class_id()
{
    //view attendance by class id function
    cout<<"View Attendance by Class ID"<<endl;
    cout<<"Enter Class ID: ";
    string class_id;
    cin>>class_id;
    cout<<"Attendance report for class "<<class_id<<endl;
}
void view_attendance_by_student_class()
{
    //view attendance by student class function
    cout<<"View Attendance by Student Class"<<endl;
    cout<<"Enter Student Class: ";
    string student_class;
    cin>>student_class;
    cout<<"Attendance report for class "<<student_class<<endl;
}        