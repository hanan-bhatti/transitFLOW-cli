#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <time.h>
#include "user.h"
#include "../utils/consoleUtils.h"
#include "../utils/fileUtils.h"
#include "../utils/asciiloader.h"
#include "../utils/filelogger.h"
#include "../auth/auth.h"
#include "../admin/admin.h"

using namespace std;

const int MAX_TRANSPORTS = 100;

bool isValidUser(const User& user) {
    return !user.email.empty() && !user.hashedPassword.empty();
}

User userLogin() {
    clearScreen();
    User user;
    
    string email = getEmail();
    user = getUserByEmail(email);

    if (user.email.empty()) {
        showToast("User not found!", 15, 4);
        writeLog("Login failed: user not found - " + email);
        return User{}; // return invalid user
    }

    string password = getPassword();
    string inputHash = Hashit(password + user.salt);

    if (inputHash == user.hashedPassword) {
        showToast("Login successful!", 0, 2);
        writeLog("User logged in: " + user.email);
        return user;
    }

    showToast("Incorrect password!", 15, 4);
    writeLog("Login failed: incorrect password - " + email);
    return User{}; // return invalid user
}

User signUp() {
    User newUser;

    string email = getEmail();
    if (isUserExsists(email)) {
        showToast("Email already registered!", 15, 4);
        slideText("Try logging instead: showing you login menu..", 0, 2);
        pauseConsole();
        writeLog("Failed registration attempt: " + email);
        clearScreen();
        return userLogin();
    }

    string username = getUsername();
    string password = getPassword();
    string salt = generateSalt(6);
    string hashedPassword = Hashit(password + salt);

    newUser.username = username;
    newUser.email = email;
    newUser.hashedPassword = hashedPassword;
    newUser.salt = salt;

    saveUser(newUser);
    showToast("Registered successfully!", 0, 2);
    writeLog("User registered: " + newUser.email);

    return newUser;
}


void userDashboard(const User &user)
{
    if (!isValidUser(user)) {
        slideText("Authentication failed. Please try again.", 15, 4);
        writeLog("Authentication failed or user data is empty");
        pauseConsole();
        return;
    }

    writeLog("User dashboard accessed by: " + user.email);
    clearScreen();

    string welcomeMessage = "Welcome, " + user.username + "!";
    showToast(welcomeMessage, 0, 2);

    string options[] = {
        "Search Transport",
        "View Schedule",
        "Sort Routes by Arrival",
        "Sort Routes by Crowd",
        "Get Alternate Routes",
        "View Live Updates",
        "View Available Seats",
        "Check Peak Hour Alerts",
        "Report Transport Issue",
        "Request Special Services",
        "Get Emergency Help",
        "View Ticket Fare",
        "Provide Feedback",
        "Save Favorite Routes",
        "Logout"};

    while (true) {
        clearScreen();
        gotoxy(0, 1);
        setColor(9);
        printTitleFor("User");
        setColor(15);
        printLine('=', 75);

        string slideMessage = "You are logged in as " + user.username + ". Enjoy your journey!\n"
                              "With email: " + user.email + "\n"
                              "With TransitFlow, you can easily manage your transport needs.\n"
                              "Use the menu to navigate through the options. (Use up and down arrow keys)";
        slideText(slideMessage, 0, 11);
        printLine('=', 75);
        cout << "\n\n";

        int choice = arrowMenu(options, 15, 10, 18, 15, 9);
        switch (choice) {
            case 0:  searchTransport(user); break;
            case 1:  viewSchedule(user); break;
            case 2:  sortRoutesByArrival(user); break;
            case 3:  sortRoutesByCrowd(user); break;
            case 4:  getAlternateRoutes(user); break;
            case 5:  viewLiveUpdates(user); break;
            case 6:  viewAvailableSeats(user); break;
            case 7:  checkPeakHourAlerts(user); break;
            case 8:  reportTransportIssue(user); break;
            case 9:  requestSpecialServices(user); break;
            case 10: getEmergencyHelp(user); break;
            case 11: viewTicketFare(user); break;
            case 12: provideFeedback(user); break;
            case 13: saveFavoriteRoutes(user); break;
            case 14:
                showToast("Logout successfully", 15, 0);
                writeLog("User logged out: " + user.email, user.username);
                return;
            default:
                showToast("Invalid choice.", 15, 4);
                break;
        }
    }
}


void searchTransport(const User& user) {
    string name, line;
    clearScreen();
    gotoxy(0, 1);
    cout << "Enter transport name: ";
    getline(cin, name);
    name = toLower(name);
    writeLog(user.username + " searched for transport: " + name);
    fstream find("../src/data/transport_data.txt", ios::in);
    if (!find.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("faild to open transport data file for " + user.username + "while acessing transport" + name);
        return;
    }

    bool found = false;
    while (getline(find, line)) {
        string lowerLine = toLower(line);
        if (lowerLine.find(name) != string::npos) {
            clearScreen();
            cout << "\n Transport Match Found: \n";
            writeLog("Transport found: " + line + " for user: " + user.username);
            printLine('-', 70);
            cout << line << endl;
            printLine('-', 70);
            found = true;
        }
    }

    if (!found) {
        showToast("Transport not found.", 15, 4);
        writeLog("Transport not found: " + name + " for user: " + user.username);
    }

    find.close();
    pauseConsole();

}

void viewSchedule(const User& user) {
    writeLog("User " + user.username + " viewed transport schedule.");
    clearScreen();
    ifstream find("../src/data/transport_data.txt");
    if (!find.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open transport data file for user: " + user.username);
        return;
    }

    printLine('-', 100);
    cout << left
         << setw(18) << "Name" << "| "
         << setw(28) << "Route" << "| "
         << setw(40) << "Timings" << "| "
         << "Crowd" << endl;
    printLine('-', 100); // printing the header of the table and line using the print line function from consoleUtils.h

    string line;
    while (getline(find, line)) {
        stringstream ss(line);
        string name, route, timings, crowd;

        getline(ss, name, '|');
        getline(ss, route, '|');
        getline(ss, timings, '|');
        getline(ss, crowd);

        cout << left
             << setw(18) << name << "| "
             << setw(28) << route << "| "
             << setw(40) << timings << "| "
             << crowd << endl;
    }

    printLine('-', 100);
    find.close();
    pauseConsole();

}

// Converts "06:00 AM" to 360 (minutes since midnight)
int parseTimeToMinutes(const string& timeStr) {
    int hours = stoi(timeStr.substr(0, 2));
    int minutes = stoi(timeStr.substr(3, 2));
    string period = timeStr.substr(6, 2);

    if (period == "PM" && hours != 12) hours += 12;
    if (period == "AM" && hours == 12) hours = 0;

    return hours * 60 + minutes;
}

void sortRoutesByArrival(const User& user) {
    TransportEntry entries[MAX_TRANSPORTS];
    int count = 0;
    writeLog("User " + user.username + " sorted transport routes by arrival time.");
    ifstream find("../src/data/transport_data.txt");
    if (!find.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open transport data file for user: " + user.username);
        return;
    }

    string line;
    while (getline(find, line) && count < MAX_TRANSPORTS) {
        stringstream ss(line);
        string name, route, timings, crowd;

        getline(ss, name, '|');
        getline(ss, route, '|');
        getline(ss, timings, '|');
        getline(ss, crowd);

        // Trim whitespace
        name = trim(name);
        route = trim(route);
        timings = trim(timings);
        crowd = trim(crowd);

        // Parse time
        size_t dashPos = timings.find('-');
        string startTime = timings.substr(0, dashPos - 1); // before space
        int startMinutes = parseTimeToMinutes(startTime);

        entries[count] = { name, route, timings, crowd, startMinutes };
        count++;
    }
    find.close();

    // Bubble sort by startTimeMinutes
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (entries[j].startTimeMinutes > entries[j + 1].startTimeMinutes) {
                TransportEntry temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }

    // Print table
    clearScreen();
    printLine('-', 100);
    cout << left
         << setw(18) << "Name" << "| "
         << setw(28) << "Route" << "| "
         << setw(40) << "Timings" << "| "
         << "Crowd" << endl;
    printLine('-', 100);

    for (int i = 0; i < count; ++i) {
        cout << left
             << setw(18) << entries[i].name << "| "
             << setw(28) << entries[i].route << "| "
             << setw(40) << entries[i].timings << "| "
             << entries[i].crowd << endl;
    }

    printLine('-', 100);
    pauseConsole();

}

// Convert crowd string to numeric value so it make sorting numeral easy
int crowdValue(const string& crowd) {
    string c = toLower(trim(crowd));
    if (c == "low crowd") return 1;
    if (c == "medium crowd") return 2;
    if (c == "high crowd") return 3;
    return 4; // unknown crowd level (put at end)
}

void sortRoutesByCrowd(const User& user) {
    TransportEntry entries[MAX_TRANSPORTS];
    int count = 0;
    writeLog("User " + user.username + " sorted transport routes by crowd level.");
    ifstream find("../src/data/transport_data.txt");
    if (!find.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open transport data file for user: " + user.username);
        return;
    }

    string line;
    while (getline(find, line) && count < MAX_TRANSPORTS) {
        stringstream ss(line);
        string name, route, timings, crowd;

        getline(ss, name, '|');
        getline(ss, route, '|');
        getline(ss, timings, '|');
        getline(ss, crowd);

        name = trim(name);
        route = trim(route);
        timings = trim(timings);
        crowd = trim(crowd);

        int level = crowdValue(crowd);

        entries[count] = { name, route, timings, crowd, level };
        count++;
    }
    find.close();

    // Bubble sort by crowd level
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (entries[j].crowdLevel > entries[j + 1].crowdLevel) {
                TransportEntry temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }

    // Print sorted table
    clearScreen();
    printLine('-', 100);
    cout << left
         << setw(18) << "Name" << "| "
         << setw(28) << "Route" << "| "
         << setw(40) << "Timings" << "| "
         << "Crowd" << endl;
    printLine('-', 100);

    for (int i = 0; i < count; ++i) {
        cout << left
             << setw(18) << entries[i].name << "| "
             << setw(28) << entries[i].route << "| "
             << setw(40) << entries[i].timings << "| "
             << entries[i].crowd << endl;
    }

    printLine('-', 100);
    pauseConsole();

}

void getAlternateRoutes(const User& user) {
    string keyword;
    clearScreen();
    gotoxy(0, 1);
    cout << "Enter a location or keyword to find alternate routes: ";
    getline(cin, keyword);
    keyword = toLower(trim(keyword));
    writeLog("User " + user.username + " searched for alternate routes with keyword: " + keyword);
    ifstream find("../src/data/transport_data.txt");
    if (!find.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open transport data file for user: " + user.username);
        return;
    }

    bool found = false;
    string line;

    clearScreen();
    printLine('-', 100);
    cout << left
         << setw(18) << "Name" << "| "
         << setw(28) << "Route" << "| "
         << setw(40) << "Timings" << "| "
         << "Crowd" << endl;
    printLine('-', 100);

    while (getline(find, line)) {
        stringstream ss(line);
        string name, route, timings, crowd;

        getline(ss, name, '|');
        getline(ss, route, '|');
        getline(ss, timings, '|');
        getline(ss, crowd);

        string routeLower = toLower(route);

        if (routeLower.find(keyword) != string::npos) {
            found = true;
            cout << left
                 << setw(18) << trim(name) << "| "
                 << setw(28) << trim(route) << "| "
                 << setw(40) << trim(timings) << "| "
                 << trim(crowd) << endl;
        }
    }

    printLine('-', 100);
    find.close();

    if (!found) {
        showToast("No matching routes found.", 15, 4);
        writeLog("No matching routes found for keyword: " + keyword + " user: " + user.username);
    } else {
        pauseConsole();
    }

}

void viewLiveUpdates(const User& user) {
    showToast("Live updates feature coming soon!", 14, 4);
    writeLog("User " + user.username + " viewed live updates.");
    pauseConsole();

}

void viewAvailableSeats(const User& user) {
    showToast("Seat availability tracking coming soon!", 14, 4);
    writeLog("User " + user.username + " viewed available seats.");
    pauseConsole();

}


//  Alerts & Reports
void checkPeakHourAlerts(const User& user) {
    time_t now = time(0);
    struct tm* local = localtime(&now);

    int hour = local->tm_hour;
    int minute = local->tm_min;
    int totalMinutes = hour * 60 + minute;

    bool isMorningPeak = (totalMinutes >= 420 && totalMinutes <= 570);   // 7:00 AM to 9:30 AM
    bool isEveningPeak = (totalMinutes >= 990 && totalMinutes <= 1140); // 4:30 PM to 7:00 PM

    if (isMorningPeak || isEveningPeak) {
        showToast("You are in Peak Hour Traffic!", 0, 12);
        writeLog("User " + user.username + " is in peak hour traffic.");
    } else {
        showToast("It's not peak hour right now.", 0, 2);
        writeLog("User " + user.username + " is not in peak hour traffic.");
    }

    pauseConsole();

}


void reportTransportIssue(const User& user) {
    string route, issue;
    clearScreen();
    gotoxy(0, 1);
    cout << "Enter route name: ";
    getline(cin, route);

    cout << "Describe the issue (e.g. delay, crowding): ";
    getline(cin, issue);

    ofstream fout("../src/data/complaints.txt", ios::app);
    if (!fout.is_open()) {
        showToast("Failed to save complaint!", 15, 4);
        return;
    }

    fout << "Route: " << route << " | Issue: " << issue << "\n";
    fout.close();

    showToast("Issue reported successfully!", 0, 2);
    writeLog("User " + user.username + " reported an issue on route: " + route);
    pauseConsole();

}

void requestSpecialServices(const User& user) {
    string options[] = {
        "Wheelchair Assistance",
        "Student Discount",
        "Senior Citizen Support",
        "Pregnancy Seat Reservation"
    };
    clearScreen();
    printBoxed("REQUEST SPECIAL SERVICES");
    int choice = arrowMenu(options, 4, 10, 8);
    string selected = options[choice];

    ofstream fout("../src/data/special_requests.txt", ios::app);
    if (!fout.is_open()) {
        showToast("Failed to save request!", 15, 4);
        return;
    }

    fout << "Requested: " << selected << "\n";
    fout.close();

    showToast("Special service requested!", 0, 2);
    writeLog("User " + user.username + " requested special service: " + selected);
    pauseConsole();

}

void getEmergencyHelp(const User& user) {
    clearScreen();
    printBoxed(" EMERGENCY CONTACT & SAFETY");

    cout << "\n Emergency Helpline: 1122\n";
    cout << " Police: 15\n";
    cout << " Ambulance: 115\n";

    cout << "\n Safety Tips:\n";
    cout << "Avoid standing near bus edges.\n";
    cout << "Stay alert during boarding.\n";
    cout << "Report suspicious activities.\n";
    cout << "Use footpaths and safe zones.\n";
    cout << "\n For emergencies, call 1122 or 15 immediately.\n";

    writeLog("User " + user.username + " accessed emergency contact information.");
    pauseConsole();

}

//  Fare & Feedback
void viewTicketFare(const User& user) {
    clearScreen();
    printBoxed("TICKET FARE INFORMATION");

    cout << "\nFixed Fare Services:\n";
    cout << "Speedo Bus: PKR 25\n";
    cout << "Metro: PKR 30\n";
    cout << "EV Shuttle: PKR 30\n";

    cout << "\n Orange Line Fare (Based on Stations):\n";
    cout << "1 - 4 stations: PKR 25\n";
    cout << "5 - 6 stations: PKR 30\n";
    cout << "7 - 8 stations: PKR 35\n";
    cout << "9 - 10 stations: PKR 40\n";
    cout << "11 or more stations: PKR 45 (max)\n";
    cout << "\nFor more details, visit our website (https://transitflow.up.railway.app) or contact customer support.\n";
    writeLog("User " + user.username + " viewed ticket fare information.");
    pauseConsole();

}



void provideFeedback(const User& user) {
    string feedback;
    clearScreen();
    gotoxy(0, 1);
    printBoxed("We value your feedback, " + user.username + ". Type it below:\n");
    getline(cin, feedback);

    ofstream fout("../src/data/reports.txt", ios::app);
    if (!fout.is_open()) {
        showToast("Failed to save feedback!", 15, 4);
        writeLog("Failed to open feedback file for user: " + user.username);
        return;
    }

    fout << "Feedback by " << user.username << " (" << user.email << "):\n";
    fout << feedback << "\n";
    fout << string(60, '-') << "\n"; // separator
    fout.close();

    showToast("Thank you for your feedback!", 0, 2);
    writeLog("User " + user.username + " provided feedback: " + feedback);
    printBoxed("Your feedback has been recorded. Thank you for helping us improve!");
    pauseConsole();

}

void saveFavoriteRoutes(const User& user) {
    string route;
    clearScreen();
    gotoxy(0, 1);
    cout << "Enter the route you want to save as favorite: ";
    getline(cin, route);
    writeLog("User " + user.username + " saveing a favorite route: " + route);
    // Count existing favorites for this user
    ifstream fin("../src/data/favorites.txt");
    int count = 0;
    string line;
    bool insideUserBlock = false;

    while (getline(fin, line)) {
        if (line.find("Favorites of " + user.username) != string::npos &&
            line.find(user.email) != string::npos) {
            insideUserBlock = true;
        } else if (insideUserBlock && line.find("------------------------------------------------------------") != string::npos) {
            break;
        } else if (insideUserBlock && line[0] >= '1' && line[0] <= '9') {
            count++;
        }
    }
    fin.close();

    // Now append the route
    ofstream fout("../src/data/favorites.txt", ios::app);
    if (!fout.is_open()) {
        showToast("Failed to save favorite!", 15, 4);
        writeLog("Failed to open favorites file for user: " + user.username);
        return;
    }

    if (count == 0) {
        fout << "Favorites of " << user.username << " (" << user.email << "):\n";
    }

    fout << (count + 1) << ". " << route << "\n";

    // If this is the first entry, close the block with separator
    if (count == 0) {
        fout << string(60, '-') << "\n";
    }

    fout.close();

    showToast(" Favorite route saved!", 0, 2);
    writeLog("User " + user.username + " saved a favorite route: " + route);
    printBoxed("Your favorite route has been saved successfully!");
    pauseConsole();

}