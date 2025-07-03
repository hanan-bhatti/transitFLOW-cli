#pragma once
#include <string>
using namespace std;

struct User {
    string username;
    string email;
    string hashedPassword;
    string salt;
};

string trim(const string &str);
void saveUser(const User& user);
bool isUserExsists(string &email);
User getUserByEmail(const string& email);
void displayAllUsers();
