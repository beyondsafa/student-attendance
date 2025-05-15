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