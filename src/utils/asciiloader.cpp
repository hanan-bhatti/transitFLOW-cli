#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

// importing custom header file
#include "asciiloader.h"

using namespace std;

// Function to print the ASCII title from a file
void printTitleFor(const string& titlename) {
    fstream titles;
    titles.open("utils/ascii_titles.txt", ios::in);
    string line;
    bool print = false;
    while (getline(titles, line)) {
        if (line == "[" + titlename + "]") {
            print = true;
            continue;
        }
        if (line == "[/" + titlename + "]") {
            break;
        }
       if (print) {
           cout << line << endl;
           Sleep(10u); // Sleep for 10 milliseconds
       }
    }
    titles.close();
}