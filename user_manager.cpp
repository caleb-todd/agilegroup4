#include "user_manager.h"
#include "crypto_util.h"
#include <fstream>
#include <sstream>
#include <random>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

const string USER_DB_FILE = "users.db";
const string DATA_DIR = "user_data";

//generates random user id
int generate_user_id(){
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(100000, 999999);
    return dis(gen);
}

//split string
vector<string> split(const string& s, char delimiter){
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while(getline(tokenStream, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}

bool create_account(const string& username, const string& password){
    //check if username exists
    ifstream inFile(USER_DB_FILE);
    string line;
    while(getline(inFile, line)){
        auto parts = split(simple_decrypt(line), ':');
        if(parts.size() >= 1 && parts[0] == username){
            return false; //username already exists
        }
    }
    inFile.close();

    //create data directory
    if(!fs::exists(DATA_DIR)){
        fs::create_directory(DATA_DIR);
    }

    //generate user ID
    int user_id = generate_user_id();

    //save credentials
    ofstream outFile(USER_DB_FILE, ios::app);
    if(!outFile) return false;

    string encrypted_entry = simple_encrypt(username + ":" + password + "+" + to_string(user_id));
    outFile << encrypted_entry << "\n";
    outFile.close();

    //create empty user data file

    UserData new_user;
    new_user.id = user_id;
    return save_user_data(new_user);
}

bool login(const string& username, const string& password, UserData& out_data){
    ifstream inFile(USER_DB_FILE);
    if (!inFile) return false;

    string line;
    while (getline(inFile, line)) {
        auto decrypted = simple_decrypt(line);
        auto parts = split(decrypted, ':');
        if (parts.size() == 3 && parts[0] == username && parts[1] == password) {
            out_data.id = stoi(parts[2]);
            
            // Load user data
            string data_file = DATA_DIR + "/" + to_string(out_data.id) + ".dat";
            ifstream data_in(data_file);
            if (data_in) {
                string encrypted_data;
                while (getline(data_in, encrypted_data)) {
                    string decrypted_data = simple_decrypt(encrypted_data);
                    try {
                        out_data.data.push_back(stoi(decrypted_data));
                    } catch (...) {
                        // Ignore invalid entries
                    }
                }
            }
            return true;
        }
    }
    return false;
}

bool save_user_data(const UserData& data) {
    string data_file = DATA_DIR + "/" + to_string(data.id) + ".dat";
    ofstream out(data_file);
    if (!out) return false;

    for (int value : data.data) {
        out << simple_encrypt(to_string(value)) << "\n";
    }
    out.close();
    return true;
}