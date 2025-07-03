#include <iostream>
#include <windows.h>
#include <conio.h>
#include "consoleUtils.h"
#include "filelogger.h"

using namespace std;

// Basic Tools
void clearScreen() {
    system("cls");
}

void pauseConsole() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void delay(int milliseconds) {
    Sleep(milliseconds);
}

void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int textColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor);
}

void setColorwithBackGround(int textColor, int bgColor) {
    int colorCode = (bgColor << 4) | textColor;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

// Design & UI
void printLine(char symbol, int length) {
    for (int i = 0; i < length; ++i) cout << symbol;
    cout << endl;
}

void printBoxed(const string& text) {
    int len = text.length() + 4;
    printLine('=', len);
    cout << "| " << text << " |\n";
    printLine('=', len);
}

void loadingBar(int sleepTime) {
    cout << "\nLoading ";
    for (int i = 0; i < 30; i++) {
        cout << char(219);
        Sleep(sleepTime);
    }
    cout << endl;
}

void slideText(const string& text, int x, int y, int delayMs) {
    gotoxy(x, y);
    for (char c : text) {
        cout << c << flush;
        Sleep(delayMs);
    }
    cout << endl;
}

void showToast(const string& message, int textColor, int bgColor) {
    // Save current cursor position
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD savedPos = csbi.dwCursorPosition;
    
    // Show toast at top-left
    setColorwithBackGround(textColor, bgColor);
    gotoxy(0, 0);
    cout << " " << message << " ";
    setColorwithBackGround(7, 0);
    Sleep(1500);
    gotoxy(0, 0);
    cout << string(message.length() + 2, ' ');
    
    // Restore cursor position
    SetConsoleCursorPosition(hConsole, savedPos);
    setColor(7); // Reset to default color
    if (textColor == 15 && bgColor == 4) {
        writeLog("Toast error message shown: " + message, "System");
    } else {
        writeLog("Toast message shown : " + message, "System");
    }
}

// Arrow Key Navigation
int arrowMenu(const string options[], int size, int x, int y) {
    int selected = 0;
    const int ENTER = 13;
    const int UP = 72, DOWN = 80;

    while (true) {
        for (int i = 0; i < size; i++) {
            gotoxy(x, y + i);
            if (i == selected) {
                setColorwithBackGround(0, 11);
                cout << " > " << options[i] << " ";
                setColorwithBackGround(7, 0);
            } else {
                cout << "   " << options[i] << " ";
            }
        }

        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            if (ch == UP && selected > 0) selected--;
            else if (ch == DOWN && selected < size - 1) selected++;
        } else if (ch == ENTER) {
            return selected;
        }
    }
}

string toLower(string str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') str[i] = str[i] + 32;
    }
    return str;
}
