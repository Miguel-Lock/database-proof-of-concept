#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <array>
#include "SQL_Interface.h"
#define v2d vector<vector<string>>

using namespace std;

int global_id;

//finds the index of target in the 0th element in arr
int find_index(string target, v2d arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i][0] == target) {
            return i; //returns the index location
        }
    }
    return INT_MIN; //if no value is found
}

//allows a registered user log in
int login() {
    int user_index, id = INT_MIN;
    string password, position, user_password = "";

    //stores all id, password, position in login_info
    v2d login_info = db_query("SELECT employee_ID, password, position FROM employee");

    //makes user enter a valid id
    while (find_index(to_string(id), login_info) == INT_MIN) {
        cout << "Enter a valid id \n>";
        cin >> id;

        if (find_index(to_string(id), login_info) == INT_MIN) {
            cout << "Not a valid ID. Please try again.\n";
        }
    }
    global_id = id;
    
    //collects password and position for user with ID id
    user_index = find_index(to_string(id), login_info);
    password = login_info[user_index][1];
    position = login_info[user_index][2];
    
    //ask user to enter the correct password until success
    while (user_password != password) {
        cout << "Enter the password associated with that ID\n>";
        cin >> user_password;

        if (user_password != password) {
            cout << "Invalid password. Try again, you fool\n";
        }
    }

    //returns an integer relating to position
    if (position == "admin") {
        return 1;
    } else if (position == "manager") {
        return 2;
    } else if (position == "cashier") {
        return 3;
    } else if (position == "grunt") {
        return 4;
    } else if (position == "customer") {
        return 5;
    }
    
    //safety in case some unexplainable error happens
    throw runtime_error("PROBLEM in final statement in  login()");
}

//Adds a user to the program
int add_user() {
    string first, last, password, role_str;
    int role_int, id, input_id;

    //Gets user name, password
    cin.ignore();
    cout << "Enter your first name:\n>";
    getline(cin, first);

    cout << "Enter your last name\n>";
    getline(cin, last);

    cout << "Enter a password:\n>";
    getline(cin, password);

    //Gets the role
    do {
        cout << "Enter a role\n";
        cout << "1. admin\n";
        cout << "2. manager\n";
        cout << "3. cashier\n";
        cout << "4. grunt\n";
        cout << "5. customer\n>";
        cin >> role_int;
    } while (role_int < 1 || role_int > 5);

    //assigns the role to a corresponding integer
    switch (role_int) {
    case 1: role_str = "admin"; break;
    case 2: role_str = "manager"; break;
    case 3: role_str = "cashier"; break;
    case 4: role_str = "grunt"; break;
    case 5: role_str = "customer"; break;
    default: throw runtime_error("PROBLEM in switch statment in add_user");
    }

    //adds the string into employee.
    db_insert("insert into employee (first_name, last_name, password, position) values ('" + first + "', '" + last + "', '" + password + "', '" + role_str + "')");
    
    //THIS CAN BE UNCOMMENTED WHEN THE DB AUTO-CREATES/INCRIMENTS ID
    
    //saves the auto-generated id
    v2d vector_id = db_query("select employee_ID from employee where first_name='" + first + "' and last_name='" + last + "' and password='" + password + "';");
    //NOTE: WILL NEED TO CHANGE THIS LINE IN CASE OF DUPLICATES
    //MUST GET THE MOST RECENT ENTRY
    id = stoi(vector_id[0][0]);
    global_id = id;
    
    //Makes user input ID 3 times so they can remember it
    cout << "This is your user ID: " << id << endl;
    cout << "You will need this to log in in the future\n";
    cout << "Do not forget this or you will be unable to login.\n";
    cout << "You will be asked to enter your ID THREE (3) times in\n \
    succession to help you plebians remember you stupid ID.\n\n";

    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            cout << "To help remember your ID, please type it in\n>";
        } else {
            cout << "Good job! You did a job a 2-year-old can do! Please type your ID in again\n>";
        }

        cin >> input_id;

        while (input_id != id) {
            cout << "You messed up and are thus more stupid than a 2-year-old. Please try again.\n";
            cout << "Please type in your *CORRECT* ID :)\n>";
            cin >> input_id;
        }
    }

    return role_int;
}

//TODO: make it pretty
void show_menu() {
    cout << "Menu:" << endl;
    v2d menu = db_query("SELECT * FROM menu_items");
    vector<string> column_names = {"Item", "Price", "Is on meal exchange", "Is_side?", "Is breakfast?", "Is lunch?", "Is dinner?"};

    for (size_t row = 0; row < menu.size(); row++) {
        for (size_t item = 0; item < menu[row].size(); item++) {
            cout << column_names[item] << ": " << menu[row][item] << "  ";
        }
        cout << endl;
    }
    cout << endl;
}

void show_timesheet() {
    cout << "Timesheet for employee id " + global_id << endl;

    v2d timesheet = db_query("SELECT * FROM timesheet WHERE employee_ID=" + to_string(global_id) + "");

    //MUST ADD TIMESHEET DATE
    vector<string> column_names = { "ID", "Date", "Scheduled start", "Scheduled end", "Scheduled number of breaks", "Scheduled number of meals", "Actual start", "Actual end", "Actual number of breaks", "Actual number of meals" };
    /*for (size_t row = 0; row < column_names.size(); row++) {
        cout << column_names[row] << " ";
    }
    cout << endl;*/

    for (size_t row = 0; row < timesheet.size(); row++) {
        for (size_t item = 0; item < timesheet[row].size(); item++) {
            cout << column_names[item] << ": " << timesheet[row][item] << "   ";
        }
        cout << endl;
    }
}

void get_date(array<int, 3> &arr) {
    int month = -5, day = -5, year = -5;

    while (year < 1900 || year > 2200) {
        cout << "Enter the year as a number\n>";
        cin >> year;

        if (year < 1900 || year > 2200) {
            cout << "Invalid year. Please try again.\n";
        }
    }

    while (month < 1 || month > 12) {
        cout << "Enter the month as a number\n>";
        cin >> month;

        if (month < 1 || month > 12) {
            cout << "Invalid month. Please try again.\n";
        }
    }

    while (day < 1 || day > 31) {
        cout << "Enter the day as a number\n>";
        cin >> day;

        if (day < 1 || day > 31) {
            cout << "Invalid day. Please try again.\n";
        }
    }

    arr = { year, month, day };
}

void get_time(array<int, 2> &arr) {
    int hour = -5, minute = -5;

    while (hour < 1 || hour > 24) {
        cout << "Enter the hour as a number\n>";
        cin >> hour;

        if (hour < 1 || hour > 24) {
            cout << "Invalid hour. Please try again.\n";
        }
    }

    while (minute < 0 || minute > 59) {
        cout << "Enter the minute as a number\n>";
        cin >> minute;

        if (minute < 0 || minute > 59) {
            cout << "Invalid minute. Please try again.\n";
        }
    }

    arr = { hour, minute };
}

void insert_self_timesheet() {
    string choice = "n";
    int year, month, day, actual_meals, actual_breaks;
    string full_date, schedule_start_time, schedule_start_date;
    string actual_start_time, actual_end_time;
    array<int, 3> date_arr;
    array<int, 2> time_arr;
    array<int, 2> actual_start;
    array<int, 2> actual_end;

    v2d timesheet;


    do {
        cout << "What day was employee " << global_id << " supposed to start?\n";
        get_date(date_arr);
        year = date_arr[0];
        month = date_arr[1];
        day = date_arr[2];

        cout << "What time was the employee supposed to begin work?\n";
        get_time(time_arr);

        schedule_start_date = to_string(year) + "-" + to_string(month) + "-" + to_string(day);
        schedule_start_time = to_string(time_arr[0]) + ":" + to_string(time_arr[1]);


        timesheet = db_query("SELECT * FROM timesheet WHERE employee_ID='" + to_string(global_id) + "' and clock_date='" + schedule_start_date + "' and scheduled_start='" + schedule_start_time + "'");


        if (timesheet.size() != 1) {
            cout << "Invalid input. You will need to try again.\n";
            cout << "Do you want to try again? Press x to give up, or c to continue\n>";
            cin >> choice;
        } else {
            choice = "z";
        }
    } while (choice != "x" && choice != "X" && choice != "z");

    if (choice == "z") {
        cout << "This is when you were supposed to start: " << timesheet[0][2] << endl;
        cout << "When did you actually start?\n";
        get_time(actual_start);
        actual_start_time = to_string(actual_start[0]) + ":" + to_string(actual_start[1]);


        cout << "This is when you were supposed to end: " << timesheet[0][3] << endl;
        cout << "When did you actually end?\n";
        get_time(actual_end);
        actual_end_time = to_string(actual_end[0]) + ":" + to_string(actual_end[1]);


        cout << "This is how many breaks you were supposed to have: " << timesheet[0][4] << endl;
        cout << "How many breaks did you actually have?\n>";
        cin >> actual_breaks;

        cout << "This is how many meals you were supposed to have: " << timesheet[0][5] << endl;
        cout << "How many meals did you actually have?\n>";
        cin >> actual_meals;

        db_insert("UPDATE timesheet SET actual_start='" + actual_start_time + \
            "', actual_end='" + actual_end_time + \
            "', actual_num_break=" + to_string(actual_breaks) + \
            ", actual_num_meal=" + to_string(actual_meals) + \
            " WHERE employee_ID=" + to_string(global_id) + \
            " AND clock_date='" + schedule_start_date + \
            "' AND scheduled_start='" + schedule_start_time + "'");
    }
}

void customer() {
    show_menu();
}

void show_inventory() {
    v2d inventory = db_query("select * from inventory");

    cout << "Inventory | expire date | Ingredient name | amount" << endl;
    for (int i = 0; i < inventory.size(); i++) {
        cout << inventory[i][0] << "    | " << inventory[i][2] <<  "  |  " << inventory[i][1] << "      | " << inventory[i][3] << endl;
    }
}

void update_inventory() {
    cout << "This is supposed to update the inventory" << endl;
}

void admin() {
    cout << "This will show admin options";
}

void cashier() {
    cout << "This will show cashier options";
}

void manager() {
    cout << "This will show manager options";
}

void grunt() {
    int option;
    string leave = "n";

    while (leave == "n") {
        do {
            cout << "Enter an option\n";
            cout << "1. See menu\n";
            cout << "2. Show timesheet\n";
            cout << "3. Insert into timesheet\n";
            cout << "4. Show Inventory\n";
            cout << "5. Update Inventory\n";
            cout << "6. Quit the program\n>";
            cin >> option;
        } while (option < 1 || option > 6);

        //assigns the role to a corresponding integer
        switch (option) {
        case 1: show_menu(); break;
        case 2: show_timesheet(); break;
        case 3: insert_self_timesheet(); break;
        case 4: show_inventory(); break;
        case 5: update_inventory(); break;
        case 6: leave = "y";
        }
    }
}

int main() {
    int login_choice = 0;
    int role;

    
    //Display initial login menu
    do {
        cout << "1. Login\n";
        cout << "2. Register new user\n";
        cout << "3. Continue without account\n>";
        cin >> login_choice;
    } while (login_choice != 1 && login_choice != 2 && login_choice !=3);

    if (login_choice == 1) {
        role = login();
    } else if (login_choice == 2) {
        role = add_user();
    } else if (login_choice == 3) {
        role = 5;
    }

    cout << endl << endl;
    

    //Opens menu depending on the role of the user
    switch (role) {
        case 1: admin(); break;
        case 2: manager(); break;
        case 3: cashier(); break;
        case 4: grunt(); break;
        case 5: customer(); break;
    }

	return 0;
}