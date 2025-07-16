#include "../include/restaurant_system.h"
#include <fstream>
#include <map>

/**
 * @brief Membuat laporan pendapatan dan statistik harian.
 *
 * Fungsi ini mengumpulkan data pelanggan yang telah membayar pada hari ini
 * dan menghitung total pendapatan untuk membuat file laporan (.txt).
 * Terdapat beberapa batasan dalam implementasi saat ini yang dicatat dalam komentar.
 */
void generateDailyReport()
{
    // Menampilkan judul untuk halaman laporan.
    displayHeading("LAPORAN HARIAN", 50, '=', true);

    // Dapatkan tanggal "hari ini" dari waktu sistem untuk pemfilteran.
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Siapkan variabel untuk menyimpan data laporan.
    vector<Customer> paidCustomersToday; // Vektor untuk menampung pelanggan yang lunas hari ini.
    double totalRevenue = 0.0;           // Variabel untuk mengakumulasi total pendapatan.

    // --- Langkah 1: Saring pelanggan yang lunas hari ini ---
    // Iterasi melalui semua pelanggan yang pernah tercatat.
    for (const auto &cust : manager.customers)
    {
        // Konversi waktu kedatangan pelanggan ke struktur tm untuk perbandingan tanggal.
        tm *arrival_tm = localtime(&cust.arrivalTime);

        // Kondisi untuk memfilter: status harus "Lunas" DAN tanggal kedatangan (hari, bulan, tahun) harus sama dengan hari ini.
        if (cust.paymentStatus == "Lunas" &&
            arrival_tm->tm_year == ltm->tm_year &&
            arrival_tm->tm_mon == ltm->tm_mon &&
            arrival_tm->tm_mday == ltm->tm_mday)
        {
            // Jika kondisi terpenuhi, masukkan pelanggan ke dalam daftar untuk laporan.
            paidCustomersToday.push_back(cust);

            // Catatan Pengembang: Blok ini sengaja dikosongkan karena ada batasan.
            // Awalnya, pendapatan bisa dihitung di sini per pelanggan.
            // Namun, karena struktur data saat ini tidak menghubungkan pesanan langsung ke pelanggan,
            // perhitungan pendapatan dilakukan secara terpisah di bawah.
            for (const auto &order : manager.orders)
            {
                // Loop ini tidak melakukan apa-apa dan ada di sini untuk menunjukkan
                // pemikiran awal yang belum bisa diimplementasikan.
            }
        }
    }

    // --- Langkah 2: Hitung total pendapatan ---
    // Catatan Pengembang: Blok kode di bawah ini menyoroti batasan signifikan dari struktur data saat ini.
    // Idealnya, kita akan mengiterasi `paidCustomersToday` dan menjumlahkan tagihan mereka.
    // Namun, karena `customerId` di meja direset setelah checkout, kita tidak bisa lagi
    // menautkan pelanggan yang sudah checkout ke pesanan mereka melalui data meja.
    for (const auto &cust : paidCustomersToday)
    {
        for (const auto &table : manager.tableManager.tables)
        {
            // Logika di sini tidak akan berfungsi seperti yang diharapkan untuk pelanggan
            // yang sudah checkout.
        }
    }

    // SOLUSI SEMENTARA: Karena batasan di atas, laporan ini akan menghitung
    // total pendapatan dari SEMUA pesanan yang pernah ada, bukan hanya pesanan hari ini.
    // Ini adalah penyederhanaan hingga struktur data diperbaiki.
    for (const auto &order : manager.orders)
    {
        totalRevenue += order.totalPrice;
    }

    // --- Langkah 3: Buat dan tulis file laporan ---
    // Buat nama file yang unik untuk laporan hari ini, contoh: "daily_report_2025-07-16.txt".
    string filename = "daily_report_" + DateTime(now, "%Y-%m-%d") + ".txt";
    ofstream reportFile(filename); // Buat objek file output dan buka file.

    // Tulis konten laporan ke dalam file.
    reportFile << "========================================\n";
    reportFile << "        LAPORAN HARIAN\n";
    reportFile << "========================================\n";
    reportFile << "Tanggal: " << DateTime(now, "%d-%m-%Y") << "\n\n";
    // Tulis total pendapatan (berdasarkan perhitungan sementara dari semua pesanan).
    reportFile << "Total Pendapatan (Semua Waktu): Rp " << fixed << setprecision(0) << totalRevenue << "\n";
    // Tulis jumlah grup pelanggan yang berhasil dilayani hari ini.
    reportFile << "Jumlah Pelanggan (Lunas Hari Ini): " << paidCustomersToday.size() << " grup\n";
    reportFile << "----------------------------------------\n";

    // Tutup file untuk memastikan semua data tersimpan.
    reportFile.close();
    // Beri konfirmasi di konsol bahwa file laporan telah berhasil dibuat.
    cout << "\n\xfb Laporan harian berhasil dibuat: " << filename << endl;
}
/**
 * @brief Membuat laporan performa menu berdasarkan jumlah item yang terjual.
 *
 * Fungsi ini mengagregasi data penjualan dari semua pesanan untuk mengetahui
 * berapa banyak porsi dari setiap item menu yang terjual. Hasilnya kemudian
 * diurutkan dari yang paling laris ke yang kurang laris dan ditulis
 * ke dalam sebuah file laporan .txt.
 */
void generateMenuPerformanceReport()
{
    // Menampilkan judul untuk halaman laporan.
    displayHeading("LAPORAN PERFORMA MENU", 50, '=', true);

    // Langkah 1: Agregasi data penjualan dari semua pesanan.
    // Gunakan std::map untuk menyimpan total kuantitas terjual untuk setiap ID item menu.
    // Key: ID item menu (int), Value: total kuantitas terjual (int).
    map<int, int> itemSales;

    // Iterasi melalui semua pesanan yang pernah ada.
    for (const auto &order : manager.orders)
    {
        // Iterasi melalui setiap item dalam pesanan tersebut.
        for (const auto &item : order.itemListData)
        {
            // Tambahkan kuantitas item ini ke total penjualan untuk ID menu yang sesuai.
            // Jika ID belum ada di map, C++ akan membuatnya secara otomatis dengan nilai default 0 lalu menambahkannya.
            itemSales[item.menuItemId] += item.qty;
        }
    }

    // Langkah 2: Urutkan data berdasarkan jumlah penjualan.
    // std::map diurutkan berdasarkan kunci (ID menu), tapi kita ingin mengurutkan berdasarkan nilai (jumlah terjual).
    // Untuk itu, kita salin data dari map ke dalam sebuah vector of pairs.
    vector<pair<int, int>> sortedItems(itemSales.begin(), itemSales.end());

    // Gunakan std::sort untuk mengurutkan vektor.
    sort(sortedItems.begin(), sortedItems.end(), [](const pair<int, int> &a, const pair<int, int> &b)
         {
             // Gunakan lambda function sebagai kriteria pengurutan.
             // 'a.second' adalah jumlah terjual item a, 'b.second' adalah jumlah terjual item b.
             // Mengembalikan true jika a.second > b.second akan mengurutkan dari terbesar ke terkecil (descending).
             return a.second > b.second; });

    // Langkah 3: Buat file laporan.
    // Buat nama file yang unik berdasarkan tanggal saat ini.
    string filename = "menu_performance_report_" + DateTime(time(0), "%Y-%m-%d") + ".txt";
    ofstream reportFile(filename); // Buat objek file output dan buka file.

    // Langkah 4: Tulis konten laporan ke dalam file.
    reportFile << "========================================\n";
    reportFile << "       LAPORAN PERFORMA MENU\n";
    reportFile << "========================================\n";

    // Tulis header kolom.
    reportFile << left << setw(25) << "Nama Item" << " | " << "Jumlah Terjual\n";
    reportFile << "----------------------------------------\n";

    // Iterasi melalui vektor yang sudah diurutkan untuk menulis datanya ke file.
    for (const auto &pair : sortedItems)
    {
        // Cari nama item berdasarkan ID-nya (pair.first).
        string itemName = "Unknown"; // Nama default jika item tidak ditemukan.
        for (const auto &menuItem : manager.menuData)
        {
            if (menuItem.id == pair.first)
            {
                itemName = menuItem.name;
                break; // Hentikan pencarian setelah item ditemukan.
            }
        }
        // Tulis baris data: nama item dan jumlah porsi yang terjual (pair.second).
        reportFile << left << setw(25) << itemName << " | " << pair.second << " porsi\n";
    }

    // Tutup file untuk menyimpan perubahan.
    reportFile.close();
    // Beri konfirmasi di konsol bahwa file laporan telah berhasil dibuat.
    cout << "\n\xfb Laporan performa menu berhasil dibuat: " << filename << endl;
}

/**
 * @brief Fungsi utama untuk menampilkan antarmuka menu laporan.
 */
void reportManagement()
{
    int choice = 0;
    do
    {
        displayHeading("MANAJEMEN LAPORAN", 50, '=', true);
        cout << "\nMENU LAPORAN:" << endl;
        cout << "1. Buat Laporan Harian" << endl;
        cout << "2. Buat Laporan Performa Menu" << endl;
        cout << "3. Buat Laporan Bulanan (Coming Soon)" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        createLine(50, "-");
        cout << "Masukan Pilihan Anda : ";
        choice = getValidInput(0, 2);

        switch (choice)
        {
        case 1:
            generateDailyReport();
            break;
        case 2:
            generateMenuPerformanceReport();
            break;
        case 3:
            cout << "\nFungsi ini belum diimplementasikan." << endl;
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
