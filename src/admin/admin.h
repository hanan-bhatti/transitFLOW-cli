#ifndef ADMIN_H
#define ADMIN_H

// admin login
bool adminLogin();

// Main Dashboard
void adminDashboard();

// Core Transport Management
void addTransportService();
void updateTransportSchedule();
void deleteTransportEntry();
void viewAllTransportServices();

// Sorting & Analytics
void sortByPeakHours();
void sortByPassengerLoad();
void analyzePeakTraffic();
void recommendRouteAdjustments();

// Real-time and Historical Data
void updateLiveDelays();
void viewTransportHistory();
void generateDailyReport();

// Feedback & Pricing
void viewComplaints();
void adjustFarePrices();

// Admin Tools
void viewLogs();
void viewAllUsers();
void logoutAdmin();

#endif
