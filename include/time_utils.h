#include "restaurant_system.h"

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

using namespace std;

// ==================================================
// TIME FUNCTIONS
// ==================================================

// Get current date and time
tm *getCurrentDateTime();

// Format a time_t value into a date or time string
string DateTime(time_t time, const char *format);

// Parse string to time_t
time_t parseDateTime(const char *datetimeString, const char *format);

// Calculate time difference
string calculateTime(time_t startTime, time_t endTime);

#endif // TIME_UTILS_H