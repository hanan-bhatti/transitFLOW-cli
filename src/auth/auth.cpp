#include <iostream>
#include <string>
#include <windows.h>
#include <time.h>
#include <cstdlib>
#include <conio.h>
#include <sstream>
#include "auth.h"
#include "../utils/consoleUtils.h"
#include "../utils/fileUtils.h"
#include "../user/user.h"
#include "../admin/admin.h"
#include "../utils/filelogger.h"

using namespace std;

string getUsername() {
    writeLog("get username was called");
    string username;
    cout << "Enter user name: ";
    getline(cin, username);
    writeLog("user entered username: " + username);
    return username;
}

string getEmail() {
    string email;
    gotoxy(0, 1);
    cout << "Enter email: ";
    getline(cin, email);
    // Trim whitespace from the email
    email = trim(email);

    // Validate the email format
    while (!isValidEmail(email)) {
        showToast("Invalid email format. Please enter a valid email.", 15, 4);
        slideText("Invalid email format. Please enter a valid email: ",0,1);
        getline(cin, email);
    }
    return email;
}

bool isValidEmail(const string& email) {
    // Minimum length check
    if (email.length() < 5) return false;

    // Find the position of '@'
    int atPos = email.find('@');
    if (atPos <= 0 || atPos == email.length() - 1) return false;

    // Find the position of '.' after the '@'
    int dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == email.length() - 1)
        return false;

    if (email.find(' ') != string::npos) return false; // no spaces are allowed in the email

    if (email.find('@', atPos + 1) != string::npos) return false; // finding if two @ exsists other than atpos @
    writeLog("got a valid mail");
    return true;
}


bool isCapsLockOn()
{
    return (GetKeyState(VK_CAPITAL) & 0x0001);
}

string getPassword()
{
    string password;
    char ch;
    bool isShowPassword = false;
    const int visiblePrefix = 2;
    bool lastState = isCapsLockOn();
    cout << "Enter password (Tab to toggle): ";
    while (true)
    {
        // Check caps lock state before getting input
        bool currentState = isCapsLockOn();
        if (currentState != lastState)
        {
            showToast(currentState ? "Caps Lock ON" : "Caps Lock OFF", 14, 4);
            writeLog(currentState ? "Caps Lock turned ON" : "Caps Lock turned OFF");
            lastState = currentState;
        }

        ch = _getch();
        if (ch == 13)
        {
            break;
        }
        else if (ch == 8)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b"; // printing backspace to remove the last typed character from screen
            }
        }
        else if (ch == 9)
        { // Tab key to toggle visibility
            isShowPassword = !isShowPassword;
            // Clear current line and reprint password
            cout << "\r";
            for (int i = 0; i < password.length() + 20; i++)
                cout << " ";
            cout << "\r";
            cout << "Enter password (Tab to toggle): ";
            writeLog("user viewed password");
            if (isShowPassword)
            {
                cout << password;
            }
            else
            {
                for (int i = 0; i < visiblePrefix && i < password.length(); i++)
                {
                    cout << password[i];
                }
                for (int i = visiblePrefix; i < password.length(); i++)
                {
                    cout << "*";
                }
            }
        }
        else if (ch > 32 && ch < 132)
        {
            password += ch;
            if (isShowPassword)
            {
                cout << ch;
            }
            else
            {
                // Show the character for 100ms, then replace with *
                cout << ch;
                cout.flush();  // Ensure character is displayed immediately
                delay(170);     // Wait for 170 milliseconds
                cout << "\b*"; // Backspace and replace with asterisk
            }
        }
    }
    return password;
}

string Hashit(const string& input) {
    unsigned long long hash = 5381;

    for (size_t i = 0; i < input.length(); ++i) {
        hash = (hash * 31) ^ (input[i] + i * 17);
    }

    // Convert to hex string for safe storage
    stringstream ss;
    ss << hex << hash;
    writeLog("converted user input " + input + " to hash " + ss.str());
    return ss.str();
}

string generateSalt(int length = 6) {
    const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    string salt = "";

    // Seed the random generator (if not already done in main)
    srand(time(0));

    for (int i = 0; i < length; ++i) {
        int index = rand() % charset.length();
        salt += charset[index];
    }
    writeLog("Generated salt" + salt);
    return salt;
}

void auth(string userChoice) {
    string options[] = { "Login", "Sign Up", "Exit" };
    int choice = arrowMenu(options, 3, 10, 10);
    clearScreen();

    if (choice == 0) { // Login
        if (userChoice == "Admin") {
            if (adminLogin()) {
                writeLog("Admin logged in successfully");
                adminDashboard();
            }
        } else {
            User user = userLogin();
            if (!user.email.empty()) userDashboard(user);
        }
    }

    else if (choice == 1) { // Sign Up
        User newUser = signUp();
        if (userChoice == "User") userDashboard(newUser);
        else showToast("Admin sign up not allowed!", 15, 4);
        writeLog("Admin tried to signup");
    }

    else if (choice == 2) {
        showToast("Exiting TransitFlow. Goodbye!", 15, 0);
        writeLog("User exited the sysrem");
        exit(0);
    }
}