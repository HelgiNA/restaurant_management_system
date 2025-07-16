#include "../include/restaurant_system.h"

// Display heading with lines
void displayHeading(const string &title, int length, char symbol, bool clearScreen)
{
    if (clearScreen)
    {
        system("cls");
    }
    createLine(length, string(1, symbol));
    cout << setw((length / 2) - (title.length() / 2)) << "" << title << endl;
    createLine(length, string(1, symbol));

    cout << "Selamat Datang, Hans" << endl;
    cout << "Tanggal: " << DateTime(time(nullptr), "%d-%m-%Y") << "  |  Waktu: " << DateTime(time(nullptr), "%H:%M") << "\n";
}

// Create line
void createLine(int length, string symbol)
{
    for (int i = 0; i < length; i++)
    {
        cout << symbol;
    }
    cout << endl;
}

// Validate number input
int getValidInput(int min, int max)
{
    int input;
    while (true)
    {
        cin >> input;

        // If input fails or is out of range
        if (cin.fail() || input < min || input > max)
        {
            cin.clear();               // Reset error flag
            cin.ignore(INT_MAX, '\n'); // Ignore invalid input
            if (max == INT_MAX)
            {
                cout << "Input tidak valid. Masukkan angka lebih dari " << min << ": ";
            }
            else
            {
                cout << "Input tidak valid. Masukkan angka antara " << min << " dan " << max << ": ";
            }
        }
        else
        {
            cin.ignore(INT_MAX, '\n'); // Ignore additional characters
            break;
        }
    }
    return input;
}

// Validate string input
string getValidString(const string &errorMessage, bool isRequired)
{
    string input;
    while (true)
    {
        getline(cin, input); // Use getline to support spaces

        if (!isRequired)
        {
            // Validate empty input or only spaces
            if (input.empty() || all_of(input.begin(), input.end(), [](char c)
                                        { return isspace(c); }))
            {
                cout << errorMessage;
            }
            else
            {
                break;
            }
        }
        else
        {
            break; // If not required, exit the loop
        }
    }
    return input;
}

void initialize()
{
    setlocale(LC_ALL, "id_ID.UTF-8");
    setlocale(LC_TIME, "id_ID.UTF-8");

    queue.head = -1;
    queue.tail = -1;

    // Initialize tables
    if (manager.tableManager.tables.empty())
    {
        manager.tableManager.size = {3, 4, 2};                        // Number of tables for each section
        manager.tableManager.capacities = {2, 4, 8};                  // Table capacities
        manager.tableManager.sections = {"Indoor", "Outdoor", "VIP"}; // Table sections
        initializeDefaultTables();                                    // Initialize default tables if none exist
    }

    // Inisialisasi meja
    if (manager.tableManager.tables.empty())
    {
        // ... (kode inisialisasi meja)
        initializeDefaultTables();
    }

    // Inisialisasi menu
    if (manager.menuData.empty())
    {
        initializeDefaultMenu(); // <<< TAMBAHKAN PANGGILAN INI
    }

    // Initialize date and time format
    strcpy(date.format, "%Y-%m-%d %H:%M:%S");
}