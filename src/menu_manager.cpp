#include "../include/restaurant_system.h"

/**
 * @brief Fungsi bantuan untuk mendapatkan nama kategori dari ID-nya.
 * @param categoryId ID integer dari kategori.
 * @return Nama kategori dalam bentuk string.
 */
string getCategoryName(int categoryId)
{
    switch (categoryId)
    {
    case 1:
        return "Pembuka Selera";
    case 2:
        return "Sajian Utama";
    case 3:
        return "Pelepas Dahaga";
    case 4:
        return "Manis Penutup";
    default:
        return "Tidak Dikenal";
    }
}

/**
 * @brief Mengisi manager.menuData dengan beberapa item default.
 */
void initializeDefaultMenu()
{
    if (!manager.menuData.empty())
        return;

    manager.menuData = {
        {101, "Lumpia Udang", 1, 35000, true},
        {102, "Salad Buah", 1, 30000, true},
        {201, "Nasi Goreng Spesial", 2, 55000, true},
        {202, "Ayam Bakar", 2, 65000, true},
        {203, "Sop Buntut", 2, 85000, false},
        {301, "Es Teh Manis", 3, 15000, true},
        {302, "Jus Alpukat", 3, 25000, true},
        {401, "Pudding Coklat", 4, 20000, true}};
    cout << "Default menu initialized." << endl;
}

/**
 * @brief Menampilkan menu yang terorganisir berdasarkan kategori.
 */
void displayMenu()
{
    displayHeading("DAFTAR MENU RESTORAN", 65, '=', true);

    if (manager.menuData.empty())
    {
        cout << "\nMenu belum tersedia." << endl;
        return;
    }

    vector<int> uniqueCategoryIds;
    for (const auto &item : manager.menuData)
    {
        if (find(uniqueCategoryIds.begin(), uniqueCategoryIds.end(), item.category) == uniqueCategoryIds.end())
        {
            uniqueCategoryIds.push_back(item.category);
        }
    }
    sort(uniqueCategoryIds.begin(), uniqueCategoryIds.end());

    for (const int categoryId : uniqueCategoryIds)
    {
        cout << "\n--- " << getCategoryName(categoryId) << " ---" << endl;
        cout << left << setw(5) << "ID"
             << " | " << setw(25) << "Nama Item"
             << " | " << setw(15) << "Harga"
             << " | " << "Status" << endl;
        createLine(65, "-");

        for (const auto &item : manager.menuData)
        {
            if (item.category == categoryId)
            {
                cout << left << setw(5) << item.id
                     << " | " << setw(25) << item.name
                     << " | Rp " << setw(12) << fixed << setprecision(0) << item.price
                     << " | " << (item.isAvailable ? "Tersedia" : "Habis") << endl;
            }
        }
    }
}

/**
 * @brief Menambahkan item menu baru berdasarkan input dari pengguna.
 */
void addMenuItem()
{
    displayHeading("TAMBAH ITEM MENU BARU", 50, '=', true);

    cout << "\nMasukkan Nama Item Baru: ";
    string name = getValidString("Nama tidak boleh kosong.");

    cout << "\nPilihan Kategori Item: " << endl;
    cout << "1. " << getCategoryName(1) << endl;
    cout << "2. " << getCategoryName(2) << endl;
    cout << "3. " << getCategoryName(3) << endl;
    cout << "4. " << getCategoryName(4) << endl;
    cout << "Masukkan Pilihan Kategori: ";
    int category = getValidInput(1, 4);

    cout << "Masukkan Harga Item: ";
    double price = getValidInput(1);

    int maxIdInCategory = 0;
    for (const auto &item : manager.menuData)
    {
        if (item.category == category && item.id > maxIdInCategory)
        {
            maxIdInCategory = item.id;
        }
    }

    int newId = (maxIdInCategory > 0) ? maxIdInCategory + 1 : (category * 100) + 1;

    MenuItem newItem = {newId, name, category, price, true};
    manager.menuData.push_back(newItem);

    cout << "\n\xfb Item '" << name << "' (ID: " << newId << ") berhasil ditambahkan ke menu." << endl;
}

/**
 * @brief Mengubah status ketersediaan item menu.
 */
void updateItemAvailability()
{
    displayHeading("UBAH KETERSEDIAAN ITEM", 50, '=', true);

    if (manager.menuData.empty())
    {
        cout << "\nMenu kosong, tidak ada item yang bisa diubah." << endl;
        return;
    }

    cout << "\nMasukkan ID Item yang akan diubah statusnya: ";
    int itemId = getValidInput(1);

    // Cari item menggunakan iterator agar bisa diubah
    auto it = find_if(manager.menuData.begin(), manager.menuData.end(),
                      [itemId](const MenuItem &item)
                      {
                          return item.id == itemId;
                      });

    if (it == manager.menuData.end())
    {
        cout << "\nItem dengan ID " << itemId << " tidak ditemukan." << endl;
        return;
    }

    // Tampilkan status saat ini
    cout << "\nItem ditemukan: " << it->name << endl;
    cout << "Status saat ini: " << (it->isAvailable ? "Tersedia" : "Habis") << endl;

    // Konfirmasi perubahan
    cout << "\nApakah Anda yakin ingin mengubah status item ini? (y/n): ";
    string confirmation = getValidString("Input tidak valid. Masukkan 'y' atau 'n'.");

    if (confirmation == "y" || confirmation == "Y")
    {
        // Ubah status
        it->isAvailable = !it->isAvailable;
        cout << "\n\xfb Status untuk item '" << it->name
             << "' berhasil diubah menjadi: " << (it->isAvailable ? "Tersedia" : "Habis") << endl;
    }
    else
    {
        cout << "\nPerubahan status dibatalkan." << endl;
    }
}

/**
 * @brief Antarmuka utama untuk manajemen menu.
 */
void menuManagement()
{
    int choice = 0;
    do
    {
        displayHeading("MANAJEMEN MENU", 50, '=', true);

        cout << "\nMENU MANAJEMEN ITEM:" << endl;
        cout << "1. Tampilkan Menu" << endl;
        cout << "2. Tambah Item Baru ke Menu" << endl;
        cout << "3. Ubah Status Ketersediaan Item" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        createLine(50, "-");
        cout << "Masukan Pilihan Anda : ";
        choice = getValidInput(0, 3);

        switch (choice)
        {
        case 1:
            displayMenu();
            break;
        case 2:
            addMenuItem();
            break;
        case 3:
            updateItemAvailability(); // Memanggil fungsi yang baru dibuat
            break;
        case 0:
            cout << "\nKembali ke Menu Utama." << endl;
            break;
        default:
            cout << "\nPilihan tidak valid. Silakan pilih antara 0-3." << endl;
            break;
        }
        system("pause");
    } while (choice != 0);
}
