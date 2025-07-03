#ifndef AUTH_H
#define AUTH_H

#include <string>

std::string getUsername();
std::string getEmail();
std::string getPassword();
std::string Hashit(const std::string& input);
std::string generateSalt(int length);
bool isValidEmail(const std::string& email);
bool isCapsLockOn();
void auth(std::string UserChoice);

#endif // AUTH_H