#ifndef UTILITY_H
#define UTILITY_H

#include "restaurant_system.h"

using namespace std;

// ==================================================
// DATA STRUCTURES
// ==================================================

// Struct untuk Item Menu
struct MenuItem
{
    int id;
    string name;
    int category;
    double price;
    bool isAvailable;
};

// Struct untuk Item dalam sebuah Pesanan
struct ItemListOrder
{
    int id;
    int menuItemId;
    int qty;
    string catatan;
};

// Struct untuk Data Pesanan
struct OrderData
{
    int id;
    int tableId;
    vector<ItemListOrder> itemListData;
    double totalPrice;
};

// Struct untuk Data Meja
struct TableData
{
    int tableId;
    int capacity;
    string section;
    bool occupied = false;
    int customerId;
    time_t timeOccupied;
};

// Struct untuk Manajer Meja
struct TableManager
{
    vector<string> sections;
    vector<int> capacities;
    vector<int> size;
    vector<TableData> tables;
};

// Struct untuk Data Pelanggan
struct Customer
{
    int id;
    string name;
    int partySize;
    string phoneNumber;
    time_t arrivalTime;
    time_t seatedTime;
    bool reservation;
    string paymentStatus = "Belum Bayar";
};

// Manajer utama untuk seluruh sistem restoran
struct RestaurantManager
{
    vector<Customer> customers;
    vector<MenuItem> menuData;
    vector<OrderData> orders;
    TableManager tableManager;
    vector<int> reservationQueue;
    vector<int> waitingQueue;
};

// Struct untuk status antrian
struct Queue
{
    int head;
    int tail;
};

// Struct untuk format Tanggal dan Waktu
struct DateAndTime
{
    char format[80] = "%Y-%m-%d %H:%M:%S";
};

#endif // UTILITY_H
