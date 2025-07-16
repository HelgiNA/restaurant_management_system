#ifndef RESTAURANT_SYSTEM_H
#define RESTAURANT_SYSTEM_H

// System includes
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <windows.h> // Untuk Sleep() di Windows
#include <vector>
#include <ctime>
#include <sstream>
#include <locale>
#include <climits>
#include <fstream> // Diperlukan untuk menulis file (file stream)

// ... tambahkan system headers lain yang sering digunakan

// Project includes dalam urutan dependency
#include "data_structures.h"
#include "utility.h"
#include "time_utils.h"
#include "table_manager.h"
#include "queue_manager.h"
#include "customer_manager.h"
#include "menu_manager.h"
#include "order_manager.h"
#include "checkout_manager.h"
#include "report_manager.h"

// Forward declarations
string DateTime(time_t time, const char *format);
tm *getCurrentDateTime();

// Global manager untuk bisa diakses di semua file
extern struct RestaurantManager manager;
extern struct Queue queue;
extern struct DateAndTime date;

#endif // RESTAURANT_SYSTEM_H
