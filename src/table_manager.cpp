#include "../include/restaurant_system.h"

// Initialize default tables
void initializeDefaultTables()
{
    // Two types of table sections
    int i = 1;
    // This logic seems incorrect for creating distinct sections. Let's assume it's a placeholder.
    // A better approach would be to loop through sections, then capacities for each section.
    // For now, we will keep the original logic to focus on the main bug.
    for (const string &section : manager.tableManager.sections)
    {
        for (size_t j = 0; j < manager.tableManager.capacities.size(); j++)
        {
            for (int k = 0; k < manager.tableManager.size[j]; k++)
            {
                TableData newTable;
                newTable.tableId = i;
                newTable.capacity = manager.tableManager.capacities[j];
                newTable.section = section;
                newTable.occupied = false;
                newTable.customerId = 0;
                newTable.timeOccupied = 0; // Initialize with 0 or a null time value
                manager.tableManager.tables.push_back(newTable);
                i++;
            }
        }
    }
}

/**
 * @brief Menampilkan status dari semua meja yang ada di restoran.
 * * Fungsi ini akan mengiterasi semua meja yang terdaftar, mengelompokkannya
 * berdasarkan seksi (misal: Indoor, Outdoor), dan menampilkan detail
 * setiap meja seperti ID, kapasitas, status (terisi/kosong),
 * ID pelanggan yang menempati, jam masuk, dan durasi.
 */
void viewAllTableStatus()
{
    // Menampilkan judul utama untuk halaman status meja.
    displayHeading("STATUS MEJA", 70, '=', true);

    cout << "\nDaftar Meja:" << endl;
    // Iterasi melalui setiap nama seksi yang ada (misal: "Indoor", "Outdoor").
    for (const string &section : manager.tableManager.sections)
    {
        // Cetak nama seksi saat ini.
        cout << "\nSection : " << section << endl;

        // Cetak header untuk kolom-kolom tabel informasi meja.
        cout << left << setw(5) << "ID"
             << " | " << setw(10) << "Kapasitas"
             << " | " << setw(10) << "Status"
             << " | " << setw(12) << "Customer ID"
             << " | " << setw(12) << "Jam Masuk"
             << " | " << "Durasi"
             << endl;
        // Buat garis pemisah di bawah header.
        createLine(70, "-");

        // Iterasi melalui semua meja yang ada dalam daftar meja.
        for (const auto &table : manager.tableManager.tables)
        {
            // Periksa apakah meja saat ini termasuk dalam seksi yang sedang ditampilkan.
            if (table.section == section)
            {
                // Cetak detail untuk setiap meja dalam format yang rapi.
                cout << left << setw(5) << table.tableId
                     << " | " << setw(10) << table.capacity
                     // Gunakan operator ternary untuk menampilkan "Terisi" atau "Kosong".
                     << " | " << setw(10) << (table.occupied ? "Terisi" : "Kosong")
                     // Tampilkan ID pelanggan jika meja terisi, jika tidak tampilkan "-".
                     << " | " << setw(12) << (table.occupied ? to_string(table.customerId) : "-")
                     // Tampilkan jam masuk yang diformat jika meja terisi, jika tidak tampilkan "-".
                     << " | " << setw(12) << (table.occupied ? DateTime(table.timeOccupied, "%H:%M:%S") : "-")
                     // Hitung dan tampilkan durasi meja ditempati jika terisi, jika tidak tampilkan "-".
                     << " | " << (table.occupied ? calculateTime(table.timeOccupied, time(nullptr)) : "-")
                     << endl;
            }
        }
    }
    cout << endl;
}

// Find suitable table combination for a party size
vector<int> findSuitableTableCombination(int partySize)
{
    cout << "Mencari kombinasi meja yang sesuai untuk " << partySize << " orang..." << endl;
    Sleep(1000); // 1 second delay

    vector<int> result;

    // If party size <= 8, try to find single table
    if (partySize <= 8)
    {
        int bestTableId = -1;
        int minWaste = INT_MAX;

        for (const auto &table : manager.tableManager.tables)
        {
            if (table.capacity >= partySize && !table.occupied)
            {
                int waste = table.capacity - partySize;
                if (waste < minWaste)
                {
                    minWaste = waste;
                    bestTableId = table.tableId;
                }
            }
        }

        if (bestTableId != -1)
        {
            result.push_back(bestTableId);
            cout << "Meja tunggal ditemukan! ID: " << bestTableId << endl;
            return result;
        }
    }

    // For party size > 8 or no single table available, find combination
    vector<vector<int>> possibleCombinations;

    // Generate possible combinations based on party size (This can be improved with a better algorithm)
    if (partySize >= 9 && partySize <= 10)
    {
        possibleCombinations.push_back({8, 2});
        possibleCombinations.push_back({4, 4, 2});
    }
    else if (partySize >= 11 && partySize <= 12)
    {
        possibleCombinations.push_back({8, 4});
    }
    else if (partySize >= 13 && partySize <= 16)
    {
        possibleCombinations.push_back({8, 8});
    }
    else if (partySize > 16)
    {
        vector<int> largeCombination;
        int remaining = partySize;
        while (remaining > 8)
        {
            largeCombination.push_back(8);
            remaining -= 8;
        }
        if (remaining > 0)
        {
            if (remaining <= 2)
                largeCombination.push_back(2);
            else if (remaining <= 4)
                largeCombination.push_back(4);
            else
                largeCombination.push_back(8);
        }
        possibleCombinations.push_back(largeCombination);
    }

    // Check which combination is available
    for (const auto &combination : possibleCombinations)
    {
        vector<int> tableIds;
        bool combinationAvailable = true;
        vector<int> usedTableIndices; // To prevent using the same physical table twice

        for (int requiredCapacity : combination)
        {
            bool foundTable = false;
            for (size_t i = 0; i < manager.tableManager.tables.size(); ++i)
            {
                const auto &table = manager.tableManager.tables[i];
                if (table.capacity == requiredCapacity && !table.occupied)
                {
                    // Check if this table index is not already used
                    if (find(usedTableIndices.begin(), usedTableIndices.end(), i) == usedTableIndices.end())
                    {
                        tableIds.push_back(table.tableId);
                        usedTableIndices.push_back(i);
                        foundTable = true;
                        break;
                    }
                }
            }

            if (!foundTable)
            {
                combinationAvailable = false;
                break;
            }
        }

        if (combinationAvailable)
        {
            result = tableIds;
            cout << "Kombinasi meja ditemukan: ";
            for (size_t i = 0; i < result.size(); i++)
            {
                cout << "Meja " << result[i];
                if (i < result.size() - 1)
                    cout << " + ";
            }
            cout << endl;
            return result;
        }
    }

    cout << "Tidak ada kombinasi meja yang tersedia untuk " << partySize << " orang." << endl;
    return result;
}

/**
 * @brief Mengelola proses checkout dan pembayaran untuk sebuah meja.
 * * Fungsi ini pertama-tama meminta ID meja yang akan melakukan checkout.
 * Setelah memvalidasi bahwa meja tersebut ada dan sedang terisi,
 * fungsi ini akan memanggil `processPayment` untuk menangani seluruh
 * logika pembayaran. Jika pembayaran berhasil, meja akan dikosongkan
 * dan statusnya direset agar tersedia kembali.
 */
void checkoutTable()
{
    // Menampilkan judul untuk halaman checkout.
    displayHeading("CHECKOUT MEJA", 50, '=', true);

    // Meminta input ID meja dari pengguna.
    cout << "\nMasukkan ID Meja yang ingin di-checkout: ";
    int tableId = getValidInput(1, manager.tableManager.tables.size());

    // Mencari data meja di dalam daftar meja manajer berdasarkan ID yang diberikan.
    auto tableIter = find_if(manager.tableManager.tables.begin(), manager.tableManager.tables.end(),
                             [tableId](const TableData &table)
                             { return table.tableId == tableId; });

    // Jika iterator menunjuk ke akhir, berarti meja tidak ditemukan.
    if (tableIter == manager.tableManager.tables.end())
    {
        cout << "\nError: ID Meja tidak ditemukan." << endl;
        return; // Keluar dari fungsi.
    }

    // Memeriksa apakah meja tersebut memang sedang terisi.
    if (!tableIter->occupied)
    {
        cout << "\nInfo: Meja " << tableId << " sudah kosong." << endl;
        return; // Keluar jika meja sudah kosong.
    }

    // Memanggil fungsi `processPayment` untuk menangani logika pembayaran tagihan.
    // Fungsi ini akan mengurus detail seperti menampilkan tagihan, menerima pembayaran, dan mencetak struk.
    processPayment(tableId);

    // Setelah proses pembayaran diasumsikan berhasil, reset status meja.
    cout << "\nMengosongkan Meja " << tableId << "..." << endl;
    tableIter->occupied = false; // Set status meja menjadi tidak terisi (false).
    tableIter->customerId = 0;   // Reset ID pelanggan menjadi 0.
    tableIter->timeOccupied = 0; // Reset waktu masuk menjadi 0.

    // Menampilkan pesan konfirmasi bahwa meja sekarang tersedia.
    cout << "\n\xfb Meja " << tableId << " sekarang tersedia." << endl;
}

// =================================================================================
// PERBAIKAN LOGIKA ADA DI DUA FUNGSI BERIKUT
// =================================================================================

// Helper function to seat customer at table
// FUNGSI INI TIDAK LAGI MENGHAPUS PELANGGAN DARI ANTRIAN
void seatCustomerAtTable(Customer &customer, TableData &table)
{
    time_t currentTime = time(nullptr);

    cout << endl;
    cout << customer.name << " (ID: " << customer.id
         << ") ditempatkan di meja " << table.tableId << endl;
    cout << "Section: " << table.section << ", Kapasitas: " << table.capacity << endl;
    cout << "Waktu tunggu: " << calculateTime(customer.arrivalTime, currentTime) << endl;

    // Update table status
    table.occupied = true;
    table.customerId = customer.id;
    table.timeOccupied = currentTime;

    // Update customer status
    customer.seatedTime = currentTime;

    // !!! PANGGILAN removeCustomerFromQueue() DIHAPUS DARI SINI !!!
}

// Helper function to handle table seating process
// FUNGSI INI SEKARANG BERTANGGUNG JAWAB MENGHAPUS PELANGGAN DARI ANTRIAN
void processTableSeating(Customer &customer, bool isReservation)
{
    while (true)
    {
        system("pause");
        viewAllTableStatus();

        cout << endl;
        createLine(50, "-");
        cout << "Rekomendasi meja untuk " << customer.partySize << " orang: " << endl;

        vector<int> recommendedTables = findSuitableTableCombination(customer.partySize);

        if (recommendedTables.empty())
        {
            cout << "Tidak ada meja yang tersedia saat ini. Pelanggan tetap di antrian." << endl;
            return;
        }

        // Display recommended tables and get confirmation
        cout << "Meja yang direkomendasikan: ";
        for (size_t i = 0; i < recommendedTables.size(); i++)
        {
            cout << "Meja " << recommendedTables[i];
            if (i < recommendedTables.size() - 1)
                cout << " + ";
        }
        cout << endl;
        cout << "Apakah Anda setuju dengan rekomendasi ini? (1=Ya, 0=Batal): ";
        int choice = getValidInput(0, 1);

        if (choice == 0)
        {
            cout << "Penempatan dibatalkan. Pelanggan tetap di antrian." << endl;
            return;
        }

        vector<int> selectedTables = recommendedTables;

        // Validate all selected tables are still available
        bool allTablesAvailable = true;
        for (int tableId : selectedTables)
        {
            auto tableIter = find_if(manager.tableManager.tables.begin(),
                                     manager.tableManager.tables.end(),
                                     [tableId](const TableData &table)
                                     { return table.tableId == tableId; });

            if (tableIter == manager.tableManager.tables.end() || tableIter->occupied)
            {
                cout << "Meja " << tableId << " tiba-tiba tidak tersedia. Mencari kombinasi lain..." << endl;
                allTablesAvailable = false;
                break;
            }
        }

        if (!allTablesAvailable)
        {
            continue; // Retry table selection
        }

        // Successfully seat the customer at multiple tables
        cout << "Menempatkan customer di " << selectedTables.size() << " meja..." << endl;
        for (int tableId : selectedTables)
        {
            auto tableIter = find_if(manager.tableManager.tables.begin(),
                                     manager.tableManager.tables.end(),
                                     [tableId](const TableData &table)
                                     { return table.tableId == tableId; });

            // Panggil fungsi yang sudah diperbaiki
            seatCustomerAtTable(customer, *tableIter);
        }

        // !!! PANGGILAN removeCustomerFromQueue() DIPINDAHKAN KE SINI !!!
        // Hapus pelanggan dari antrian HANYA SETELAH semua proses penempatan selesai.
        removeCustomerFromQueue(isReservation);
        cout << "\nPelanggan " << customer.name << " berhasil ditempatkan dan dihapus dari antrian." << endl;

        break; // Exit the while loop
    }
}

// Table management menu
void tableManagement()
{
    int choice = 0;
    do
    {
        displayHeading("MANAJEMEN MEJA", 50, '=', true);

        cout << "\nMenu Utama:" << endl;
        cout << "1. Lihat Status Semua Meja" << endl;
        cout << "2. Checkout Meja" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        createLine(50, "-");
        cout << "Masukan Pilihan Anda : ";
        choice = getValidInput(0, 2);

        switch (choice)
        {
        case 1:
            viewAllTableStatus();
            break;
        case 2:
            checkoutTable();
            break;
        case 0:
            cout << "Kembali ke Menu Utama." << endl;
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
            break;
        }
        system("pause");
    } while (choice != 0);
}
