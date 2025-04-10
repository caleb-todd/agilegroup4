#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include <sstream>
#include <iomanip>
#include <random>

using namespace std;
namespace fs = filesystem;

void ensure_data_directory(){
    if(fs::exists("user_data")){
        fs::create_directory("user_data");
    }
}

string hash_password(const string& password){
    hash<string> hasher;
    size_t hash = hasher(password);

    stringstream ss;
    ss << hex << hash;
    return ss.str();
}
int generate_user_id(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100000, 999999);
    return dis(gen);
}
bool doesuserexist(const string& username){
    ensure_data_directory();
    for(const auto& entry : fs::directory_iterator("user_data")){
        if(entry.is_regular_file()){
            ifstream file(entry.path());
            string line;

            if(getline(file, line)){
                if(line == username){
                    file.close();
                    return true;
                }
            }
            file.close();
        }
    }
    return false;
}
void createaccount(string username, string password){
    ensure_data_directory();

    int user_id = generate_user_id();
    while(fs::exists("user_data/" + to_string(user_id) + ".dat")){
        user_id = generate_user_id();
    }

    ofstream file("user_data/" + to_string(user_id) + ".dat");

    file << username << "\n";
    file << hash_password(password) << "\n";

    file << "0.0\n";

    file.close();
}
int validatelogin(const string& username, const string& password){
    ensure_data_directory();

    for(const auto& entry : fs::directory_iterator("user_data")){
        if(entry.is_regular_file()){
            ifstream file(entry.path());
            string stored_username;
            string stored_password_hash;

            //read username and password hash
            if(getline(file, stored_username) && getline(file, stored_password_hash)){
                if(stored_username == username && stored_password_hash == hash_password(password)){
                    //get userid
                    string filename = entry.path().filename().string();
                    filename = filename.substr(0, filename.find('.'));
                    int user_id = stoi(filename);

                    file.close();
                    return user_id;
                }
            }
            file.close();
        }
    }
    //return user id, -1 if incorrect login details
    return -1;
}
vector<float> get_user_accounts(int user_id){
    vector<float> accounts;
    ifstream file("user_data/" + to_string(user_id) + ".dat");
    if(!file) return accounts;

    //skip username and password
    string line;
    getline(file, line);
    getline(file, line);

    //read all account balances
    while(getline(file,line)){
        accounts.push_back(stof(line));
    }
    file.close();
    return accounts;
}
void displaydashboard(int user_id){
    //display dashboard
    vector<float> accounts = get_user_accounts(user_id);

    cout << endl << "=== ACCOUNT DASHBOARD ===" << endl;
    cout << "Your user id: " << user_id << endl;
    cout << "Accounts: " << endl;
    for(size_t i = 0; i<accounts.size(); ++i){
        cout << "Account " << (i+1) << ": $" << accounts[i] << endl;
    }

    cout << "=========================" << endl << endl;
}

void update_user_accounts(int user_id, const vector<float>& updatedAccounts)
{
    string filename = "user_data/" + to_string(user_id) + ".dat";
    ifstream infile(filename);
    vector<string> lines;
    string line;

    while (getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();

    int balanceStart = 2;

    while (balanceStart < lines.size()) 
    {
        stof(lines[balanceStart]);
        lines.erase(lines.begin() + balanceStart);
    }

    for (int i = 0; i < updatedAccounts.size(); ++i) {
        ostringstream ss;
        ss << fixed << setprecision(2) << updatedAccounts[i];
        lines.insert(lines.begin() + balanceStart + i, ss.str());
    }

    ofstream outfile(filename);
    for (const auto& l : lines) {
        outfile << l << '\n';
    }
    outfile.close();
}

void transferFunds(int user_id, int accountToWithdrawFrom, int accountToDepositTo, float amount)
{
    vector<float> accounts = get_user_accounts(user_id);

    if (accountToWithdrawFrom-1 >= accounts.size() || accountToDepositTo-1 >= accounts.size()) {
        cout << "Invalid account index.\n";
        return;
    }
    if (accounts[accountToWithdrawFrom-1] < amount) {
        cout << "Insufficient funds.\n";
        return;
    }

    accounts[accountToWithdrawFrom-1] -= amount;
    accounts[accountToDepositTo-1] += amount;

    update_user_accounts(user_id, accounts);

    cout << "Transfer complete.\n";
}
void calculateInterestForAllAccounts() {
    ensure_data_directory();
    cout << "\n=== INTEREST PROFIT CALCULATION ===" << endl;
    cout << "Enter annual interest rate (in percent): ";
    float rate;
    cin >> rate;
    cout << "Enter period in months: ";
    int period;
    cin >> period;
    
    float totalInterest = 0.0;
    float totalPrincipal = 0.0;
    
    // Iterate through all user data files
    for(const auto& entry : fs::directory_iterator("user_data")){
        if(entry.is_regular_file()){
            ifstream file(entry.path());
            string temp;
            getline(file, temp);
            getline(file, temp);
            string line;
            while(getline(file, line)){
                try {
                    float balance = stof(line);
                    totalPrincipal += balance;
                    float interest = balance * (rate / 100.0) * (period / 12.0);
                    totalInterest += interest;
                } catch(exception& e){
                    // Ignore if error
                }
            }
            file.close();
        }
    }
    
    cout << "\nTotal principal across all user accounts: $" << totalPrincipal << endl;
    cout << "Total interest profit over " << period << " month(s) at " << rate << "% annual rate: $" << totalInterest << "\n" << endl;
}
int main() {
    while(1){
        cout << "Please select an option:" << endl;
        cout << "1 - Create Account" << endl;
        cout << "2 - Login" << endl;
        //cout << "3 - Move Money between accounts" << endl;
        cout << "3 - Calculate Interest Profit" << endl;
        cout << "4 - Exit" << endl;


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
                usernamealreadyexists = doesuserexist(username);
                if(usernamealreadyexists == false){
                        cout << "Please enter a password:";
                        string password;
                        cin >> password;
                        cout << "Please confirm your password:";
                        string password2;
                        cin >> password2;
                        if(password == password2){
                            //create account
                            createaccount(username, password);
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
        else if(x == 2){
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
                int userid = validatelogin(username, password);
                if(userid == -1){
                    cout << "Invalid username or password, try again" << endl;
                    successfullogin = false;
                }else{
                    successfullogin = true;
                }

                if(successfullogin)
                {
                     //successfullogin = true;
                    //cout << username << " : " << password << endl;
                    cout << "Successfully logged in!" << endl;
                    //do logic to go to main dashboard
                    bool loggedin = true;
                    while(loggedin == true){
                        displaydashboard(userid);
                        int y;
                        cout << "Options:" << endl;
                        cout << "1 - Transfer funds" << endl;
                        cout << "2 - Create new checking account" << endl;
                        cout << "3 - Deposit money" << endl;
                        cout << "4 - Withdraw money" << endl;
                        cout << "5 - Logout" << endl;
                        cin >> y;
                        if(y == 1){
                            int account1, account2;
                            float amount;
                            //successfullogin = true;
                            //cout << username << " : " << password << endl;
                            //cout << "Successfully logged in!" << endl;
                            //do logic to go to main dashboard
                            displaydashboard(userid);
                            vector<float> accounts = get_user_accounts(userid);

                            cout << "Account to take money from: ";
                            cin >> account1;

                            cout << "Account to add money to: ";
                            cin >> account2;

                            cout << "How much money do you want to move: ";
                            cin >> amount;

                            transferFunds(userid, account1, account2, amount);
                        }else if(y == 2){
                            cout << "Are you sure you want to create a new account? (y/n)" << endl;
                            char c;
                            cin >> c;
                            if(c == 'y'){
                                ifstream file("user_data/" + to_string(userid) + ".dat");
                                vector<string> lines;
                                string line;
                                while(getline(file, line)){
                                    lines.push_back(line);
                                }
                                file.close();

                                ofstream overwritefile("user_data/" + to_string(userid) + ".dat");

                                for(const auto& existing_lines : lines){
                                    overwritefile << existing_lines << "\n";
                                }
                                overwritefile << "0.0\n";
                                overwritefile.close();
                                cout << "Account successfully created!" << endl;
                            }else{

                            }
                            
                        }else if(y == 3){
                            vector<float> accounts = get_user_accounts(userid);
                            cout << "Which account would you like to deposit money to?" << endl;
                            int acc;
                            cin >> acc;
                            if(acc-1 >= accounts.size()){
                                cout << "Invalid account index.\n";
                            }else{
                                cout << "How much are you depositing?" << endl;
                                float amt;
                                cin >> amt;
                                if(amt > 0){
                                    accounts[acc-1] += amt;
                                    update_user_accounts(userid, accounts);
                                    cout << "Deposit complete.\n";
                                }
                            }
                        }
                        else if(y == 4){
                            vector<float> accounts = get_user_accounts(userid);
                            cout << "Which account would you like to withdraw money from?" << endl;
                            int acc;
                            cin >> acc;
                            if(acc-1 >= accounts.size()){
                                cout << "Invalid account index.\n";
                            }else{
                                cout << "How much are you withdrawing? " << endl;
                                float amt;
                                cin >> amt;
                                if(amt > 0 && accounts[acc-1] > amt){
                                    accounts[acc-1] -= amt;
                                    update_user_accounts(userid, accounts);
                                    cout << "Withdrawal complete.\n";
                                }
                            }
                        }else if(y == 5){
                            cout << "Logging out..." << endl;
                            loggedin = false;
                        }else{
                            cout << "Invalid option, try again" << endl;
                        }
                    }
                    
                }
            }
        }
        else if(x == 3){
            calculateInterestForAllAccounts();
        }
        else if(x == 4){
            cout << "Exiting.." << endl;
            return 0;
        }else{
            cout << "Invalid option, try again" << endl;
        }
    }
}