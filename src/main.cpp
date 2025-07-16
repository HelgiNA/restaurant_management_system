#include "../include/restaurant_system.h"

using namespace std;
// Global variables
const string CORRECT_PASSWORD = "admin";
const int MAX_LOGIN_ATTEMPTS = 3;

RestaurantManager manager;
Queue queue;
DateAndTime date;

bool login()
{
    displayHeading("SISTEM MANAJEMEN RESTORAN", 50, '=', true);

    for (int attempts = 0; attempts < MAX_LOGIN_ATTEMPTS; ++attempts)
    {
        cout << "\nMasukan Password (" << MAX_LOGIN_ATTEMPTS - attempts << " percobaan tersisa): ";
        string passwd = getValidString();

        if (passwd == CORRECT_PASSWORD)
        {
            cout << "Login berhasil!" << endl;
            system("pause");
            return true; // Login sukses, keluar dari fungsi
        }
        else
        {
            cout << "Password salah. Silakan coba lagi." << endl;
        }
    }

    cout << "\nAnda telah gagal login sebanyak " << MAX_LOGIN_ATTEMPTS << " kali." << endl;
    return false; // Gagal setelah semua percobaan habis
}

int main()
{
    int choice = 0;
    initialize(); // Initialize queue and tables

    if (!login())
    {
        return 0;
    }

    do
    {
        displayHeading("SISTEM MANAJEMEN RESTORAN", 50, '=', true);

        cout << "\nMENU UTAMA:" << endl;
        cout << "1. Manajemen Antrian" << endl;
        cout << "2. Manajemen Meja" << endl;
        cout << "3. Manajemen Pesanan" << endl;
        cout << "4. Manajemen Menu" << endl;
        cout << "5. Laporan" << endl;
        cout << "0. Keluar" << endl;
        createLine(50, "-");
        cout << "Masukan Pilihan Anda : ";
        choice = getValidInput(0, 5);

        switch (choice)
        {
        case 1:
            queueManagement();
            break;
        case 2:
            tableManagement();
            break;
        case 3:
            orderManagement();
            // Call order management function here
            break;
        case 4:
            menuManagement();
            // Call menu management function here
            break;
        case 5:
            reportManagement();
            // Call report function here
            break;
        case 0:
            cout << "Keluar dari program." << endl;
            break;
        default:
            cout << "Pilihan tidak valid. Silakan pilih antara 0-5." << endl;
            break;
        }
        system("pause");
    } while (choice != 0);

    return 0;
}