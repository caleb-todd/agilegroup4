#include "user_manager.h"
#include <iostream>
#include <limits>
using namespace std;

void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void display_menu() {
    cout << "\n1. Create Account\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Choose an option: ";
}

void create_account_flow() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (create_account(username, password)) {
        cout << "Account created successfully!\n";
    } else {
        cout << "Failed to create account (username may already exist).\n";
    }
}

void login_flow() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    UserData user;
    if (login(username, password, user)) {
        cout << "Login successful!\n";
        cout << "Your user ID: " << user.id << "\n";
        cout << "Your data: ";
        
        if (user.data.empty()) {
            cout << "No data stored.\n";
        } else {
            for (int value : user.data) {
                cout << value << " ";
            }
            cout << "\n";
        }

        // Simple data editing
        cout << "\n1. Add a number\n";
        cout << "2. Clear all data\n";
        cout << "3. Logout\n";
        cout << "Choose an option: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            int new_value;
            cout << "Enter a number to add: ";
            cin >> new_value;
            user.data.push_back(new_value);
            save_user_data(user);
            cout << "Data saved.\n";
        } else if (choice == 2) {
            user.data.clear();
            save_user_data(user);
            cout << "Data cleared.\n";
        }
    } else {
        cout << "Invalid username or password.\n";
    }
}

int main() {
    while (true) {
        display_menu();
        int choice;
        cin >> choice;
        clear_input();

        switch (choice) {
            case 1:
                create_account_flow();
                break;
            case 2:
                login_flow();
                break;
            case 3:
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}