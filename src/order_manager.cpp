#include "../include/restaurant_system.h"
#include <iostream>
#include <algorithm> // Diperlukan untuk std::find_if

/**
 * @brief Menampilkan semua pesanan yang ada dengan detail item-itemnya.
 *
 * Fungsi ini akan mengiterasi semua pesanan yang pernah dibuat. Untuk setiap
 * pesanan, ia akan menampilkan informasi header (ID Pesanan, No Meja),
 * diikuti oleh daftar detail dari setiap item yang dipesan (ID, Nama, Jumlah, Catatan),
 * dan diakhiri dengan total harga pesanan tersebut.
 */
void viewOrders()
{
    // Menampilkan judul utama untuk halaman daftar pesanan.
    displayHeading("DAFTAR SEMUA PESANAN", 70, '=', true);

    // Periksa apakah ada pesanan yang tersimpan. Jika tidak, tampilkan pesan.
    if (manager.orders.empty())
    {
        cout << "\nBelum ada pesanan yang dibuat." << endl;
        return; // Keluar dari fungsi jika tidak ada pesanan.
    }

    // Iterasi melalui setiap objek pesanan dalam daftar pesanan manajer.
    for (const auto &order : manager.orders)
    {
        // Mencetak garis pembatas atas dan informasi header untuk pesanan saat ini.
        cout << "\n======================================================================" << endl;
        cout << " Pesanan ID: " << order.id
             << " |  Meja No: " << order.tableId << endl;
        cout << "----------------------------------------------------------------------" << endl;

        // Mencetak header kolom untuk daftar item dalam pesanan.
        cout << left << setw(5) << " ID"
             << " | " << setw(25) << "Nama Item"
             << " | " << setw(5) << "Jml"
             << " | " << "Catatan" << endl;
        createLine(70, "-"); // Membuat garis pemisah di bawah header.

        // Iterasi melalui setiap item yang ada dalam pesanan saat ini.
        for (const auto &itemInOrder : order.itemListData)
        {
            // Inisialisasi nama item dengan pesan default. Ini untuk menangani kasus jika data menu terhapus.
            string itemName = "Item Tidak Ditemukan";

            // Cari nama item di data menu utama (manager.menuData) berdasarkan ID item.
            for (const auto &menuItem : manager.menuData)
            {
                // Jika ID item di pesanan cocok dengan ID di daftar menu...
                if (menuItem.id == itemInOrder.menuItemId)
                {
                    itemName = menuItem.name; // ...ambil namanya.
                    break;                    // Hentikan pencarian karena item sudah ditemukan.
                }
            }

            // Cetak detail untuk setiap item: ID, nama, jumlah, dan catatan.
            cout << left << setw(5) << itemInOrder.menuItemId
                 << " | " << setw(25) << itemName
                 << " | " << setw(5) << itemInOrder.qty
                 << " | " << itemInOrder.catatan << endl;
        }

        // Mencetak garis pembatas bawah dan total harga untuk pesanan ini.
        cout << "----------------------------------------------------------------------" << endl;
        cout << " Total Harga: Rp " << fixed << setprecision(0) << order.totalPrice << endl;
    }
}
/**
 * @brief Membuat pesanan baru untuk meja yang dipilih.
 *
 * Fungsi ini mengelola seluruh alur pembuatan pesanan, mulai dari
 * pemilihan meja, validasi meja, penambahan item menu ke dalam pesanan,
 * hingga finalisasi dan penyimpanan pesanan.
 */
void createOrder()
{
    // Menampilkan judul untuk halaman pembuatan pesanan.
    displayHeading("BUAT PESANAN BARU", 60, '=', true);

    // --- Langkah 1: Pilih dan Validasi Meja ---
    cout << "\nMasukkan Nomor Meja untuk membuat pesanan: ";
    int tableId = getValidInput(1); // Meminta input ID meja yang valid dari pengguna.

    // Cari meja berdasarkan tableId yang diinput.
    auto tableIt = find_if(manager.tableManager.tables.begin(), manager.tableManager.tables.end(),
                           [tableId](const TableData &t)
                           { return t.tableId == tableId; });

    // Periksa apakah meja ditemukan. Jika tidak, tampilkan error dan keluar.
    if (tableIt == manager.tableManager.tables.end())
    {
        cout << "\nError: Meja dengan nomor " << tableId << " tidak ditemukan." << endl;
        return;
    }

    // Periksa apakah meja sudah terisi. Pesanan hanya bisa dibuat untuk meja yang terisi.
    if (!tableIt->occupied)
    {
        cout << "\nError: Meja " << tableId << " masih kosong. Tidak dapat membuat pesanan." << endl;
        return;
    }

    // --- Langkah 2: Inisialisasi Pesanan Baru ---
    OrderData newOrder;         // Buat objek pesanan baru.
    newOrder.tableId = tableId; // Atur ID meja untuk pesanan ini.
    newOrder.totalPrice = 0;    // Inisialisasi total harga menjadi 0.

    // Buat ID unik untuk pesanan baru.
    // Jika belum ada pesanan, gunakan ID awal 202501. Jika sudah ada, tambahkan 1 dari ID terakhir.
    newOrder.id = manager.orders.empty() ? 202501 : manager.orders.back().id + 1;

    int itemCounter = 1; // Inisialisasi penghitung untuk ID item dalam pesanan ini.

    // --- Langkah 3: Loop untuk Menambahkan Item Menu ---
    while (true)
    {
        system("cls"); // Bersihkan layar konsol untuk tampilan yang lebih rapi.
        displayMenu(); // Tampilkan daftar menu agar pengguna mudah memilih.
        cout << "\n--- Menambahkan Item untuk Pesanan #" << newOrder.id << " di Meja " << tableId << " ---" << endl;

        cout << "\nMasukkan ID Item Menu (atau 0 untuk selesai memesan): ";
        int menuItemId = getValidInput(0); // Minta input ID menu.

        // Jika pengguna memasukkan 0, hentikan loop untuk menambah item.
        if (menuItemId == 0)
            break;

        // Cari item menu berdasarkan ID yang diinput.
        auto menuItemIt = find_if(manager.menuData.begin(), manager.menuData.end(),
                                  [menuItemId](const MenuItem &m)
                                  { return m.id == menuItemId; });

        // Jika item menu tidak ditemukan, beri tahu pengguna dan ulangi loop.
        if (menuItemIt == manager.menuData.end())
        {
            cout << "\nError: Item menu dengan ID " << menuItemId << " tidak ditemukan." << endl;
            system("pause");
            continue; // Lanjut ke iterasi berikutnya.
        }

        // Periksa ketersediaan item menu.
        if (!menuItemIt->isAvailable)
        {
            cout << "\nMaaf, '" << menuItemIt->name << "' sedang habis." << endl;
            system("pause");
            continue; // Lanjut ke iterasi berikutnya.
        }

        // Minta input kuantitas dan catatan tambahan untuk item.
        cout << "Masukkan Jumlah untuk '" << menuItemIt->name << "': ";
        int qty = getValidInput(1); // Kuantitas minimal adalah 1.

        cout << "Masukkan Catatan (opsional, tekan enter jika tidak ada): ";
        string catatan = getValidString("", true); // Memungkinkan input kosong.

        // Tambahkan item yang sudah divalidasi ke dalam daftar item pesanan.
        newOrder.itemListData.push_back({itemCounter++, menuItemId, qty, catatan});
        // Akumulasikan total harga pesanan.
        newOrder.totalPrice += menuItemIt->price * qty;

        cout << "\n\xfb " << qty << "x " << menuItemIt->name << " berhasil ditambahkan." << endl;
        system("pause"); // Jeda agar pengguna bisa membaca konfirmasi.
    }

    // --- Langkah 4: Finalisasi Pesanan ---
    // Periksa apakah ada item yang berhasil ditambahkan ke pesanan.
    if (newOrder.itemListData.empty())
    {
        cout << "\nPesanan dibatalkan karena tidak ada item yang ditambahkan." << endl;
    }
    else
    {
        // Jika ada item, simpan pesanan baru ke dalam daftar pesanan utama.
        manager.orders.push_back(newOrder);
        cout << "\n\xfb Pesanan #" << newOrder.id << " untuk Meja " << tableId << " berhasil dibuat." << endl;
        cout << "Total Harga Sementara: Rp " << fixed << setprecision(0) << newOrder.totalPrice << endl;
    }
}

/**
 * @brief Antarmuka utama untuk manajemen pesanan.
 */
void orderManagement()
{
    int choice = 0;
    do
    {
        displayHeading("MANAJEMEN PESANAN", 50, '=', true);

        cout << "\nMENU MANAJEMEN PESANAN:" << endl;
        cout << "1. Lihat Semua Pesanan" << endl;
        cout << "2. Buat Pesanan Baru" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        createLine(50, "-");
        cout << "Masukan Pilihan Anda : ";
        choice = getValidInput(0, 2);

        switch (choice)
        {
        case 1:
            viewOrders();
            break;
        case 2:
            createOrder(); // Memanggil fungsi yang baru dibuat
            break;
        case 0:
            cout << "\nKembali ke Menu Utama." << endl;
            break;
        default:
            cout << "\nPilihan tidak valid." << endl;
            break;
        }
        system("pause");
    } while (choice != 0);
}
