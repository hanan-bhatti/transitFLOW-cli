#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <string>

// Console I/O helpers
void clearScreen();
void pauseConsole();
void delay(int milliseconds);
void gotoxy(int x, int y);
void setColor(int textColor);
void setColorwithBackGround(int textColor, int bgColor);

// Animations and UI
void loadingBar(int sleepTime);
void printLine(char symbol, int length);
void printBoxed(const std::string& text);
void slideText(const std::string& text, int x, int y, int delayMs = 10);
void showToast(const std::string& message, int textColor = 15, int bgColor = 4);
int arrowMenu(const std::string options[], int size, int x = 10, int y = 5);

// utility function

std::string toLower(std::string str);

#endif
