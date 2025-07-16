#include "../include/restaurant_system.h"

// Get current date and time
tm *getCurrentDateTime()
{
    time_t now = time(0);
    return localtime(&now);
}

// Format a time_t value into a date or time string
string DateTime(time_t time, const char *format)
{
    char buffer[90];
    struct tm *timeinfo = localtime(&time);
    strftime(buffer, sizeof(buffer), format, timeinfo);
    return buffer;
}

// Parse string to time_t
time_t parseDateTime(const char *datetimeString, const char *format)
{
    struct tm timeinfo = {};
    istringstream ss(datetimeString);
    ss >> get_time(&timeinfo, format);
    if (ss.fail())
    {
        cerr << "Parse failed\n";
        return -1;
    }
    return mktime(&timeinfo);
}

// Calculate time difference
string calculateTime(time_t startTime, time_t endTime)
{
    time_t remainingTime = endTime - startTime;
    if (remainingTime < 0)
    {
        return "00:00";
    }
    else if (remainingTime <= 60)
    {
        return to_string(remainingTime) + " Detik";
    }
    else if (remainingTime <= 3600)
    {
        return to_string(remainingTime / 60) + " Menit " + to_string(remainingTime % 60) + " Detik";
    }
    else if (remainingTime <= 86400)
    {
        return to_string(remainingTime / 3600) + " Jam " +
               to_string((remainingTime % 3600) / 60) + " Menit " +
               to_string(remainingTime % 60) + " Detik";
    }
    return ""; // Added default return
}