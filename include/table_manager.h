
#ifndef TABLE_MANAGER_H
#define TABLE_MANAGER_H

#include "restaurant_system.h"

// ==================================================
// TABLE MANAGEMENT
// ==================================================

// Initialize default tables
void initializeDefaultTables();

// View all table status
void viewAllTableStatus();

// Find suitable table for a party size
vector<int> findSuitableTableCombination(int partySize);

// Checkout a table by ID
void checkoutTable();

// Table management menu
void tableManagement();

void processTableSeating(Customer &customer, bool isReservation);

int getTableChoice(int recommendedTableId);

void seatCustomerAtTable(Customer &customer, TableData &table, bool isReservation);

#endif // TABLE_MANAGER_H