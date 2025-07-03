#include <iostream>
#include <fstream>
#include <iomanip>
#include "consoleUtils.h"
#include "fileUtils.h"
#include "filelogger.h"

string trim(const string& str) {
    int start = 0;
    int end = str.length() - 1;

    // Trim from start
    while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')) {
        start++;
    }

    // Trim from end
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r')) {
        end--;
    }

    return str.substr(start, end - start + 1);
}


void saveUser(const User& user) {
    ofstream fout("../src/data/users.txt", ios::app); // append mode opening file where last user data is stored so not overwritting the exsisting one this is a simple approah than using arrays to store all data and than write in the file
    if (!fout.is_open()) {
        showToast("Failed to open users file!", 15, 4);
        writeLog("Failed to open users file for writing", "System");
        return;
    }

    fout << left
         << setw(15) << user.username << "| "
         << setw(25) << user.email << "| "
         << setw(30) << user.hashedPassword << "| "
         << user.salt << "\n";

    fout.close();
    writeLog("User saved: " + user.username + " | " + user.email);
}

bool isUserExsists(string &email) {
    ifstream find("../src/data/users.txt");
    if (!find.is_open()) {
        showToast("Failed to open users file!", 15, 4);
        writeLog("Failed to open users file for reading", "System");
        return false;
    }

    string line;
    while (getline(find, line)) {
        if (line.find(email) != string::npos) {
            find.close();
            return true;
        }
    }

    find.close();
    writeLog("User not found: " + email, "System");
    return false;
}

User getUserByEmail(const string& email) {
    ifstream find("../src/data/users.txt");
    User foundUser;

    if (!find.is_open()) {
        showToast("Failed to open users file!", 15, 4);
        writeLog("Failed to open users file for reading", "System");
        return foundUser;
    }

    string line;
    while (getline(find, line)) {
        stringstream ss(line);
        string username, emailInFile, hash, salt;

        getline(ss, username, '|');
        getline(ss, emailInFile, '|');
        getline(ss, hash, '|');
        getline(ss, salt);

        // Clean up trailing and leading spaces for each
        username = trim(username);
        emailInFile = trim(emailInFile);
        hash = trim(hash);
        salt = trim(salt);

        // Now compare safely
        if (emailInFile == email) {
            foundUser.username = username;
            foundUser.email = emailInFile;
            foundUser.hashedPassword = hash;
            foundUser.salt = salt;
            break;
        }
    }

    find.close();
    writeLog("User found by email: " + email + " - Username: " + foundUser.username);
    return foundUser;
}


void displayAllUsers() { // this function is for admin use only so it just displays all users in a formatted table and don't return and get anything
    ifstream find("../src/data/users.txt");
    if (!find.is_open()) {
        showToast("Failed to open users file!", 15, 4);
        return;
    }

    printLine('-', 100);
    cout << left
         << setw(18) << "Username" << "| "
         << setw(28) << "Email" << "| "
         << setw(40) << "Hashed Password" << "| "
         << "Salt" << endl;
    printLine('-', 100); // printing the header of the table and line using the print line function from consoleUtils.h

    string line;
    while (getline(find, line)) {
        stringstream ss(line);
        string username, email, hashedPassword, salt;

        getline(ss, username, '|');
        getline(ss, email, '|');
        getline(ss, hashedPassword, '|');
        getline(ss, salt);

        cout << left
             << setw(18) << username << "| "
             << setw(28) << email << "| "
             << setw(40) << hashedPassword << "| "
             << salt << endl;
    }

    printLine('-', 100);
    find.close();
}