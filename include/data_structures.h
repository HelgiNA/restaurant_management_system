
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "restaurant_system.h"
using namespace std;

// ==================================================
// UTILITY FUNCTIONS
// ==================================================

// Display heading with lines
void displayHeading(const string &title, int length = 54, char symbol = '=', bool clearScreen = true);

// Create line
void createLine(int length, string symbol = "=");

// Validate number input
int getValidInput(int min = 1, int max = INT_MAX);

// Validate string input
string getValidString(const string &errorMessage = "Input tidak valid. Masukkan teks yang benar.", bool isRequired = false);

// Initialize the system
void initialize();

#endif // DATA_STRUCTURES_H