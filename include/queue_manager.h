#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include "restaurant_system.h"

// ==================================================
// QUEUE MANAGEMENT
// ==================================================

// Check if queue is empty
int isEmpty(bool reservation = false);

// Check if queue is full
int isFull();

// Generate unique ID for customers
int generateID();

// Add customer to queue
void addToQueue(string name, int partySize, string phoneNumber, bool reservation, time_t currentTime = time(nullptr));

// Remove customer from queue
void dequeue();

// Clear all data from queue
void clearQueue();

// Queue management menu
void queueManagement();

void removeCustomerFromQueue(bool isReservation);

#endif // QUEUE_MANAGER_H