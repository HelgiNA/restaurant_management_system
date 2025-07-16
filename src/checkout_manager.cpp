#include "../include/restaurant_system.h"

/**
 * @brief Menghitung total tagihan dari semua pesanan untuk sebuah meja.
 *
 * @param tableId ID dari meja yang tagihannya akan dihitung.
 * @return double Total tagihan dari semua pesanan yang terkait dengan tableId.
 */
double calculateBill(int tableId)
{
    // Inisialisasi variabel untuk menyimpan total tagihan, dimulai dari 0.
    double totalBill = 0.0;

    // Iterasi melalui seluruh daftar pesanan yang ada dalam sistem.
    for (const auto &order : manager.orders)
    {
        // Periksa apakah pesanan saat ini adalah milik meja yang sedang dihitung tagihannya.
        if (order.tableId == tableId)
        {
            // Jika ya, tambahkan total harga dari pesanan ini ke akumulasi total tagihan.
            totalBill += order.totalPrice;
        }
    }
    // Kembalikan total tagihan akhir yang telah diakumulasikan.
    return totalBill;
}

/**
 * @brief Mencetak struk pembayaran ke file .txt.
 */
void printReceipt(int tableId, double totalBill)
{
    // Cari pelanggan yang menempati meja
    auto tableIt = find_if(manager.tableManager.tables.begin(), manager.tableManager.tables.end(),
                           [tableId](const TableData &t)
                           { return t.tableId == tableId; });

    if (tableIt == manager.tableManager.tables.end())
        return; // Seharusnya tidak terjadi

    auto customerIt = find_if(manager.customers.begin(), manager.customers.end(),
                              [&](const Customer &c)
                              { return c.id == tableIt->customerId; });

    // Buat nama file yang unik berdasarkan waktu
    time_t now = time(0);
    string filename = "receipt_meja_" + to_string(tableId) + "_" + to_string(now) + ".txt";

    ofstream receiptFile(filename); // Membuat dan membuka file

    if (!receiptFile.is_open())
    {
        cout << "\nError: Gagal membuat file struk." << endl;
        return;
    }

    // Tulis header struk
    receiptFile << "========================================\n";
    receiptFile << "         RESTORAN SEJAHTERA\n";
    receiptFile << "========================================\n";
    receiptFile << "Tanggal: " << DateTime(now, "%d-%m-%Y %H:%M:%S") << "\n";
    if (customerIt != manager.customers.end())
    {
        receiptFile << "Pelanggan: " << customerIt->name << "\n";
    }
    receiptFile << "Meja No: " << tableId << "\n";
    receiptFile << "----------------------------------------\n\n";

    // Tulis detail pesanan
    for (const auto &order : manager.orders)
    {
        if (order.tableId == tableId)
        {
            receiptFile << "Pesanan #" << order.id << "\n";
            for (const auto &item : order.itemListData)
            {
                string itemName = "Item Dihapus";
                for (const auto &menuItem : manager.menuData)
                {
                    if (menuItem.id == item.menuItemId)
                    {
                        itemName = menuItem.name;
                        break;
                    }
                }
                receiptFile << " - " << item.qty << "x " << itemName << "\n";
            }
        }
    }

    // Tulis total
    receiptFile << "\n----------------------------------------\n";
    receiptFile << "TOTAL TAGIHAN: Rp " << fixed << setprecision(0) << totalBill << "\n";
    receiptFile << "----------------------------------------\n";
    receiptFile << "         TERIMA KASIH!\n";
    receiptFile << "========================================\n";

    receiptFile.close();
    cout << "\n\xfb Struk pembayaran berhasil dicetak ke file: " << filename << endl;
}

/**
 * @brief Memproses pembayaran untuk sebuah meja.
 *
 * Fungsi ini mengelola seluruh alur pembayaran. Mulai dari menghitung
 * total tagihan, menampilkan opsi pembayaran, mencetak struk, hingga
 * memperbarui status pembayaran pelanggan menjadi lunas.
 *
 * @param tableId ID dari meja yang akan melakukan pembayaran.
 */
void processPayment(int tableId)
{
    // Menampilkan judul untuk halaman proses pembayaran.
    displayHeading("PROSES PEMBAYARAN", 50, '=', true);

    // Langkah 1: Panggil fungsi untuk menghitung total tagihan dari semua pesanan di meja ini.
    double totalBill = calculateBill(tableId);

    // Jika total tagihan adalah 0 atau kurang, tidak ada yang perlu dibayar.
    if (totalBill <= 0)
    {
        cout << "\nTidak ada tagihan untuk meja " << tableId << "." << endl;
        // Meskipun tidak ada tagihan, meja tetap akan dikosongkan setelah fungsi ini selesai.
        return;
    }

    // Langkah 2: Identifikasi pelanggan yang akan membayar.
    // Cari data meja terlebih dahulu untuk mendapatkan ID pelanggan.
    auto tableIt = find_if(manager.tableManager.tables.begin(), manager.tableManager.tables.end(),
                           [tableId](const TableData &t)
                           { return t.tableId == tableId; });

    // Gunakan ID pelanggan dari meja untuk mencari data lengkap pelanggan.
    auto customerIt = find_if(manager.customers.begin(), manager.customers.end(),
                              [&](Customer &c)
                              { return c.id == tableIt->customerId; });

    // Jika data pelanggan tidak ditemukan, proses tidak dapat dilanjutkan.
    if (customerIt == manager.customers.end())
    {
        cout << "\nError: Pelanggan untuk meja ini tidak ditemukan." << endl;
        return;
    }

    // Langkah 3: Tampilkan informasi tagihan dan minta pilihan metode pembayaran.
    cout << "\nTotal Tagihan untuk Meja " << tableId << ": Rp " << fixed << setprecision(0) << totalBill << endl;
    cout << "Metode Pembayaran:" << endl;
    cout << "1. Tunai" << endl;
    cout << "2. Kartu Kredit/Debit" << endl;
    cout << "3. QRIS" << endl;
    cout << "Pilih metode pembayaran: ";
    int choice = getValidInput(1, 3); // Menerima input pilihan antara 1 sampai 3.

    // Langkah 4: Simulasikan jeda waktu untuk proses pembayaran.
    cout << "\nMemproses pembayaran..." << endl;
    Sleep(1500); // Jeda selama 1.5 detik untuk memberi kesan proses sedang berjalan.

    // Langkah 5: Panggil fungsi untuk mencetak struk pembayaran ke file.
    printReceipt(tableId, totalBill);

    // Langkah 6: Perbarui status pembayaran pada data pelanggan.
    customerIt->paymentStatus = "Lunas";

    // Langkah 7: Tampilkan pesan konfirmasi keberhasilan.
    cout << "\n\xfb Pembayaran berhasil diproses. Status pelanggan '" << customerIt->name << "' telah lunas." << endl;
}