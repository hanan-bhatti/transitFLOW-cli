#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <time.h>
#include "../utils/consoleUtils.h"
#include "../utils/fileUtils.h"
#include "../utils/asciiloader.h"
#include "../utils/filelogger.h"
#include "../auth/auth.h"
#include "../user/user.h"
#include "admin.h"

using namespace std;

const int MAX_TRANSPORTS = 100;

bool adminLogin() {
    string email = getEmail();

    if (email != "hanan@transitflow.com") {
        showToast("Not authorized as Admin!", 15, 4);
        writeLog(email + "tried to accesed admin panel");
        return false;
    }

    string password = getPassword();
    string salt = "Vtmjgd";
    string inputHash = Hashit(password + salt);
    string correctHash = Hashit("hanan123" + salt); // hardcoded password hash

    if (inputHash == correctHash) {
        showToast("Admin login successful!", 0, 2);
        writeLog("Admin login was succesfull");
        return true;
    }

    showToast("Incorrect admin password!", 15, 4);
    writeLog("someone trying to access admin with wrong password");
    return false;
}


void adminDashboard() {
    writeLog("admin panel accessed");
    clearScreen();
    gotoxy(0, 1);
    printTitleFor("Admin");
    slideText("Welcome, Admin!", 10, 9);

    const string menu[] = {
        "Add a New Transport Service",
        "Update Transport Schedule",
        "Delete Transport Entry",
        "View All Transport Services",
        "Sort Services by Peak Hours",
        "Sort Services by Passenger Load",
        "Analyze Peak Hour Traffic",
        "Recommend Route Adjustments",
        "Update Live Delays & Traffic Reports",
        "View Transport History & Past Modifications",
        "Generate Daily Transport Reports",
        "View Passenger Complaints & Reports",
        "Adjust Fare Prices Based on Demand",
        "View Logs",
        "View All Users",
        "Logout Admin"
    };

    while (true) {
        int choice = arrowMenu(menu, 16, 10, 11);

        switch (choice) {
            case 0: addTransportService(); break;
            case 1: updateTransportSchedule(); break;
            case 2: deleteTransportEntry(); break;
            case 3: viewAllTransportServices(); break;
            case 4: sortByPeakHours(); break;
            case 5: sortByPassengerLoad(); break;
            case 6: analyzePeakTraffic(); break;
            case 7: recommendRouteAdjustments(); break;
            case 8: updateLiveDelays(); break;
            case 9: viewTransportHistory(); break;
            case 10: generateDailyReport(); break;
            case 11: viewComplaints(); break;
            case 12: adjustFarePrices(); break;
            case 13: viewLogs(); break;
            case 14: viewAllUsers(); break;
            case 15:
                showToast("Logging out...", 0, 4);
                return;
        }

        pauseConsole();
        clearScreen();
    }
}


void addTransportService() {
    clearScreen();
    gotoxy(0, 1);
    printBoxed("ADD NEW TRANSPORT SERVICE");

    string name, route, timings, crowd;

    // Input details
    cout << "\nEnter Transport Name: ";
    getline(cin, name);
    cout << "Enter Route (e.g., Station A to Station B): ";
    getline(cin, route);
    cout << "Enter Timings (e.g., 06:00 AM - 09:00 AM): ";
    getline(cin, timings);
    cout << "Enter Crowd Level (Low/Medium/High): ";
    getline(cin, crowd);

    // Basic Validation
    if (name.empty() || route.empty() || timings.empty() || crowd.empty()) {
        showToast("All fields are required!", 15, 4);
        return;
    }

    ofstream fout("../src/data/transport_data.txt", ios::app);
    if (!fout.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open the transport data file for admin", "Admin");
        return;
    }

    // Save in formatted way
    fout << left
         << setw(18) << trim(name) << "|"
         << setw(28) << trim(route) << "|"
         << setw(40) << trim(timings) << "|"
         << trim(crowd) << "\n";

    fout.close();
    showToast("Transport added successfully!", 0, 2);
    writeLog("New transport service added: " + name + " | " + route + " | " + timings + " | " + crowd, "Admin");
    pauseConsole();
}


void updateTransportSchedule() {
    clearScreen();
    gotoxy(0, 1);
    printBoxed("UPDATE TRANSPORT SCHEDULE");

    string targetName;
    cout << "\nEnter the Transport Name to update: ";
    getline(cin, targetName);
    targetName = toLower(trim(targetName));

    ifstream fin("../src/data/transport_data.txt");
    ofstream fout("../src/data/temp.txt"); // temp file to write updates
    if (!fin.is_open() || !fout.is_open()) {
        showToast("Failed to open file!", 15, 4);
        writeLog("Failed to open transport data file for updating schedule", "Admin");
        return;
    }

    string line;
    bool found = false;
    string name, route, timings, crowd;

    while (getline(fin, line)) {
        stringstream ss(line);

        getline(ss, name, '|');
        getline(ss, route, '|');
        getline(ss, timings, '|');
        getline(ss, crowd);

        string nameLower = toLower(trim(name));

        if (nameLower == targetName) {
            found = true;
            cout << "\nCurrent Entry:\n" << line << "\n";

            cout << "Enter new Route: ";
            getline(cin, route);
            cout << "Enter new Timings (e.g., 06:00 AM - 09:00 AM): ";
            getline(cin, timings);
            cout << "Enter new Crowd Level (Low/Medium/High): ";
            getline(cin, crowd);

            fout << trim(name) << "|" << trim(route) << "|" << trim(timings) << "|" << trim(crowd) << "\n";
        } else {
            fout << line << "\n";
        }
    }

    fin.close();
    fout.close();

    if (!found) {
        showToast("Transport not found!", 15, 4);
        remove("../src/data/temp.txt"); // discard temp file if nothing updated
        return;
    }

    remove("../src/data/transport_data.txt"); // delete old file
    writeLog("Removed old transport data file for update", "Admin");
    writeLog("Transport schedule updated for: " + targetName, "Admin");
    writeLog("New details: " + targetName + " | " + route + " | " + timings + " | " + crowd, "Admin");
    // Rename temp file to original
    rename("../src/data/temp.txt", "../src/data/transport_data.txt"); // rename temp to original
    writeLog("Renamed temp file to transport_data.txt after update", "Admin");
    showToast("Schedule updated successfully!", 0, 2);
    writeLog("Transport schedule updated: " + targetName + " | " + route + " | " + timings + " | " + crowd, "Admin");
}


void deleteTransportEntry() {
    clearScreen();
    gotoxy(0, 1);
    printBoxed("DELETE TRANSPORT ENTRY");

    string targetName;
    cout << "\nEnter the Transport Name to delete: ";
    getline(cin, targetName);
    targetName = toLower(trim(targetName));

    ifstream fin("../src/data/transport_data.txt");
    ofstream fout("../src/data/temp.txt");

    if (!fin.is_open() || !fout.is_open()) {
        showToast("Failed to open file!", 15, 4);
        writeLog("Failed to open transport data file for deletion", "Admin");
        return;
    }

    string line;
    bool found = false;

    while (getline(fin, line)) {
        stringstream ss(line);
        string name, route, timings, crowd;

        getline(ss, name, '|');
        getline(ss, route, '|');
        getline(ss, timings, '|');
        getline(ss, crowd);

        string nameLower = toLower(trim(name));

        if (nameLower == targetName) {
            found = true;
            continue; // skip writing this line to delete it
        }

        fout << line << "\n"; // keep all other lines
    }

    fin.close();
    fout.close();

    if (!found) {
        showToast("Transport not found!", 15, 4);
        remove("../src/data/temp.txt"); // discard temp file
        writeLog("Transport not found for deletion: " + targetName, "Admin");
    } else {
        remove("../src/data/transport_data.txt");
        rename("../src/data/temp.txt", "../src/data/transport_data.txt");
        showToast("Transport entry deleted!", 0, 2);
        writeLog("Transport entry deleted: " + targetName, "Admin");
    }
}


void viewAllTransportServices() {
    ifstream find("../src/data/transport_data.txt");
    if (!find.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open transport data file for viewing all services", "Admin");
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
    writeLog("Viewed all transport services", "Admin");
    pauseConsole();
}

// Sorting & Analytics
void sortByPeakHours() {
    TransportEntry entries[MAX_TRANSPORTS];
    int count = 0;

    ifstream find("../src/data/transport_data.txt");
    if (!find.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open transport data file for sorting by peak hours", "Admin");
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
    writeLog("Sorted transport services by peak hours", "Admin");
    pauseConsole();
}

void sortByPassengerLoad() {
    showToast("comming soon");
    writeLog("Attempted to sort transport services by passenger load", "Admin");
}

void analyzePeakTraffic() {
    TransportEntry entries[MAX_TRANSPORTS];
    int count = 0;

    ifstream find("../src/data/transport_data.txt");
    if (!find.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open transport data file for analyzing peak traffic", "Admin");
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

        size_t dashPos = timings.find('-');
        if (dashPos == string::npos) continue;

        string startTime = timings.substr(0, dashPos - 1);
        int startMinutes = parseTimeToMinutes(startTime);

        entries[count] = { name, route, timings, crowd, startMinutes };
        count++;
    }
    find.close();

    // Define peak time ranges
    int morningStart = 420;  // 7:00 AM
    int morningEnd = 570;    // 9:30 AM
    int eveningStart = 990;  // 4:30 PM
    int eveningEnd = 1140;   // 7:00 PM

    int morningCount = 0, eveningCount = 0;

    for (int i = 0; i < count; ++i) {
        int t = entries[i].startTimeMinutes;
        if (t >= morningStart && t <= morningEnd) morningCount++;
        else if (t >= eveningStart && t <= eveningEnd) eveningCount++;
    }

    clearScreen();
    printBoxed("PEAK HOUR TRAFFIC ANALYSIS");
    cout << "\nTotal Transports Found: " << count << "\n";
    cout << "Morning Peak (7:00 AM - 9:30 AM): " << morningCount << " services\n";
    cout << "Evening Peak (4:30 PM - 7:00 PM): " << eveningCount << " services\n";

    if (morningCount > eveningCount)
        cout << "\nInsight: More load in morning peak hours.\n";
    else if (eveningCount > morningCount)
        cout << "\nInsight: More load in evening peak hours.\n";
    else
        cout << "\nInsight: Equal traffic in both peak hours.\n";

    pauseConsole();
}

void recommendRouteAdjustments() {
    TransportEntry entries[MAX_TRANSPORTS];
    int count = 0;

    ifstream fin("../src/data/transport_data.txt");
    if (!fin.is_open()) {
        showToast("Failed to open transport data file!", 15, 4);
        writeLog("Failed to open transport data file for route adjustments", "Admin");
        return;
    }

    string line;
    while (getline(fin, line) && count < MAX_TRANSPORTS) {
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
    fin.close();

    clearScreen();
    printBoxed("ROUTE ADJUSTMENT RECOMMENDATIONS");

    int suggestions = 0;
    for (int i = 0; i < count; ++i) {
        if (entries[i].crowdLevel >= 3) { // High or unknown
            suggestions++;
            cout << "\n[" << suggestions << "] Route: " << entries[i].route
                 << "\n     Service: " << entries[i].name
                 << "\n     Timings: " << entries[i].timings
                 << "\n     Current Load: " << entries[i].crowd
                 << "\n  Suggestion: Add more buses or increase frequency.\n";
        }
    }

    if (suggestions == 0) {
        showToast("No route adjustments needed currently!", 10, 4);
    } else {
        cout << "\nTotal Recommendations: " << suggestions << "\n";
        pauseConsole();
    }
    writeLog("Route adjustment recommendations viewed", "Admin");
}


// Real-time and Historical Data
void updateLiveDelays() {
    showToast("comming soon");
    writeLog("Attempted to update live delays and traffic reports", "Admin");
}

void viewTransportHistory() {
    showToast("comming soon");
    writeLog("Attempted to view transport history and past modifications", "Admin");
}

void generateDailyReport() {
    showToast("comming soon");
    writeLog("Attempted to generate daily report", "Admin");
}

// Feedback & Pricing
void viewComplaints() {
    ifstream fin("../src/data/complaints.txt");
    if (!fin.is_open()) {
        showToast("Failed to open complaints file!", 15, 4);
        writeLog("Failed to open complaints file for viewing", "Admin");
        return;
    }

    clearScreen();
    printBoxed("PASSENGER COMPLAINTS & REPORTS");

    string line;
    int count = 0;

    while (getline(fin, line)) {
        if (!line.empty()) {
            cout << ++count << ". " << line << endl;
        }
    }

    if (count == 0) {
        showToast("No complaints found.", 15, 4);
    } else {
        printLine('-', 60);
        cout << "Total complaints: " << count << endl;
    }

    fin.close();
    writeLog("Complaints viewed", "Admin");
    pauseConsole();
}


void adjustFarePrices() {
    showToast("comming soon");
    writeLog("Attempted to adjust fare prices based on demand", "Admin");
}

// Admin Tools
void viewLogs() {
    clearScreen();
    gotoxy(0, 1);
    printBoxed(" ADMIN ACTIVITY LOGS ");

    ifstream logs("../src/logs/activity.log");
    if (!logs.is_open()) {
        showToast("Log file not found!", 15, 4);
        writeLog("Failed to open activity log file", "Admin");
        return;
    }

    string line;
    bool isEmpty = true;

    while (getline(logs, line)) {
        cout << line << endl;
        isEmpty = false;
    }

    if (isEmpty) {
        showToast("No activity logs to show.", 14, 4);
        writeLog("No activity logs found", "Admin");
    }

    logs.close();
}


void viewAllUsers() {
    clearScreen();
    gotoxy(0, 1);
    displayAllUsers();
}

void logoutAdmin() {
    showToast("Logging out as Admin...", 15, 0);
    writeLog("Admin logged out", "Admin");
    delay(1000); // Short delay before returning to main menu
    clearScreen();
}
