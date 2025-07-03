#ifndef USER_H
#define USER_H

#include <string>
#include "../utils/fileUtils.h" // contains the `User` struct
using namespace std;

struct TransportEntry {
    string name;
    string route;
    string timings;
    string crowd;
    int startTimeMinutes; // for sorting
    int crowdLevel; // for sorting by crowd
};

// user login
User userLogin();
User signUp();

// utility functions
int parseTimeToMinutes(const string& time);
int crowdValue(const string &crowd);
bool isValidUser(const User& user);

//  Main Dashboard
void userDashboard(const User& user);

//  Transport Functions
void searchTransport(const User& user);
void viewSchedule(const User& user);
void sortRoutesByArrival(const User& user);
void sortRoutesByCrowd(const User& user);
void getAlternateRoutes(const User& user);
void viewLiveUpdates(const User& user);
void viewAvailableSeats(const User& user);

//  Alerts & Reports
void checkPeakHourAlerts(const User& user);
void reportTransportIssue(const User& user);
void requestSpecialServices(const User& user);
void getEmergencyHelp(const User& user);

//  Fare & Feedback
void viewTicketFare(const User& user);
void provideFeedback(const User& user);
void saveFavoriteRoutes(const User& user);

#endif // USER_H
