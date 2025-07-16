#ifndef CUSTOMER_MANAGER_H
#define CUSTOMER_MANAGER_H

#include "restaurant_system.h"

// ==================================================
// CUSTOMER MANAGEMENT
// ==================================================

// Add customer to queue
void addCustomerToQueue(bool reservation = false);

// View queue of customers
void viewQueue();

// Seat next customer in queue
void seatNextCustomer();

bool checkCustomerArrival(const Customer &customer, bool isReservation);

#endif // CUSTOMER_MANAGER_H