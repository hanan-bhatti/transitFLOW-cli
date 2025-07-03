#include <iostream>
#include <string>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <iomanip>

// importing custom header file
#include "utils/asciiloader.h"
#include "utils/consoleUtils.h"
#include "utils/fileUtils.h"
#include "utils/filelogger.h"
#include "admin/admin.h"
#include "auth/auth.h"
#include "user/user.h"

using namespace std;

int main() {
    writeLog("TransitFlow system started");
    while (true) {
        clearScreen();
        printTitleFor("Title");
        slideText("Welcome to TransitFlow", 10, 15);

        string options[] = {
            "Admin",
            "User",
            "Exit"
        };

        int choice = arrowMenu(options, 3, 10, 17);
        clearScreen();

        switch (choice) {
            case 0:
                auth("Admin");
                writeLog("someone trying to login as Admin");
                break;
            case 1:
                auth("User");
                writeLog("someone trying to login as User");
                break;
            case 2:
                showToast("Exiting TransitFlow. Goodbye!", 15, 0);
                exit(0);
            default:
                showToast("Invalid input!", 15, 4);
        }

        delay(1000); // Short pause before looping again
    }

    return 0;
}