#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include "fileLogger.h"

using namespace std;

string getCurrentTimestamp() {
    time_t now = time(0);
    tm* t = localtime(&now);

    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %I:%M:%S %p", t);
    return string(buffer);
}

void writeLog(const string& action, const string& actor) {
    ofstream fout("../src/logs/activity.log", ios::app);

    if (!fout.is_open()) {
        cerr << "Failed to open log file!" << endl;
        return;
    }

    fout << "[" << getCurrentTimestamp() << "] ";
    fout << "[" << actor << "] ";
    fout << action << endl;

    fout.close();
}
