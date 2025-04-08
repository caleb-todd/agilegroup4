#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <vector>
using namespace std;

struct UserData {
    int id;
    vector<int> data;
};

bool create_account(const string& username, const string& password);
bool login(const string& username, const string& password, UserData& out_data);
bool save_user_data(const UserData& data);

#endif