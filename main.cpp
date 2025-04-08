#include <iostream>

using namespace std;

int main() {
    while(1){
        cout << "Please select an option:" << endl;
        cout << "1 - Create Account" << endl;
        cout << "2 - Login" << endl;
        cout << "3 - Exit" << endl;

        int x;
        cin >> x;
        //cout << x << endl;
        if(x == 1){
            bool accountcreated = false;
            while(accountcreated == false){
                cout << "Creating new account..:" << endl;
                cout << "Please enter a new username:";
                bool usernamealreadyexists = false;
                string username;
                cin >> username;
                //do logic to check if a user has already claimed this username
                if(usernamealreadyexists == false){
                        cout << "Please enter a password:";
                        string password;
                        cin >> password;
                        cout << "Please confirm your password:";
                        string password2;
                        cin >> password2;
                        if(password == password2){
                            //create account
                            accountcreated = true;
                        }else{
                            cout << "Passwords do not match.. please try again." << endl;
                        }
                }else{
                    cout << "Username already taken.. please try again." << endl;
                }
            }
            cout << "Account created!" << endl;
        }
        if(x == 2){
            bool successfullogin = false;
            while(successfullogin == false){
                cout << "Please enter a new username:";
                bool usernamealreadyexists = false;
                string username;
                cin >> username;
                cout << "Please enter a password:";
                string password;
                cin >> password;
                //do logic check to see if login is correct
                successfullogin = true;
                cout << username << " : " << password << endl;
                cout << "Successfully logged in!" << endl;
                //do logic to go to main dashboard
            }
        }
        if(x == 3){
            cout << "Exiting.." << endl;
            return 0;
        }
    }
}