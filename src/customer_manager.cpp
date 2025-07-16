#include "../include/restaurant_system.h"

void addCustomerToQueue(bool reservation)
{
    displayHeading((reservation) ? "TAMBAH PELANGGAN KE ANTRIAN RESERVASI" : "TAMBAH PELANGGAN KE ANTRIAN TUNGGU", 50, '=', true);

    cout << "\n--------------------------------" << endl;
    cout << "Masukan Nama Pelanggan: ";
    string name = getValidString();
    cout << "Masukan Jumlah Orang: ";
    int partySize = getValidInput(1, 10);
    cout << "Masukan Nomor Telepon (Opsional): ";
    string phoneNumber = getValidString("Masukan Nomor Telepon yang valid: ", true);

    addToQueue(name, partySize, phoneNumber, reservation); // Add customer to queue

    cout << "\n\xfb " << manager.customers[queue.tail].name << " (ID: " << manager.customers[queue.tail].id << ") ditambahkan ke antrian "
         << (reservation ? "prioritas" : "tunggu") << "." << endl;
    cout << "Ada 27 meja tersedia untuk grup Anda sekarang!" << endl;
}

void viewQueue()
{
    // View Queue List
    displayHeading("LIHAT ANTRIAN", 50, '=', true);

    cout << "\nDaftar Antrian:" << endl;
    if (isEmpty(true))
    {
        cout << "\nAntrian Reservasi: Kosong" << endl;
    }
    else
    {
        cout << "Antrian Reservasi: " << endl;
        for (size_t i = 0; i < manager.reservationQueue.size(); i++)
        {
            int customerId = manager.reservationQueue[i];
            auto it = find_if(manager.customers.begin(), manager.customers.end(),
                              [customerId](const Customer &c)
                              { return c.id == customerId; });

            if (it != manager.customers.end())
            {
                cout << i + 1 << ". ID: " << it->id << ", Nama: " << it->name
                     << ", Jumlah Orang: " << it->partySize << endl;
                cout << "   Tiba: " << DateTime(it->arrivalTime, "%H:%M")
                     << ", Menunggu : " << calculateTime(it->arrivalTime, time(nullptr)) << endl;
            }
        }
    }
    cout << endl;

    if (isEmpty(false))
    {
        cout << "Antrian Tunggu: Kosong" << endl;
    }
    else
    {
        cout << "Antrian Tunggu: " << endl;
        for (size_t i = 0; i < manager.waitingQueue.size(); i++)
        {
            int customerId = manager.waitingQueue[i];
            auto it = find_if(manager.customers.begin(), manager.customers.end(),
                              [customerId](const Customer &c)
                              { return c.id == customerId; });

            if (it != manager.customers.end())
            {
                cout << i + 1 << ". ID: " << it->id << ", Nama: " << it->name
                     << ", Jumlah Orang: " << it->partySize << endl;
                cout << "   Tiba: " << DateTime(it->arrivalTime, "%H:%M")
                     << ", Menunggu : " << calculateTime(it->arrivalTime, time(nullptr)) << endl;
            }
        }
    }
    cout << endl;
    cout << "Total Pelanggan dalam Antrian: " << queue.tail + 1 << endl;
}

// Helper function to check customer arrival
bool checkCustomerArrival(const Customer &customer, bool isReservation)
{
    const int MAX_ATTEMPTS = 3;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt)
    {
        displayHeading("SEAT NEXT CUSTOMER", 50, '=', true);

        // Display customer information
        cout << "\nPelanggan berikutnya   : " << customer.name << " (ID: " << customer.id << ")" << endl;
        cout << "Jumlah Orang           : " << customer.partySize << endl;
        cout << "Nomor Telepon          : " << customer.phoneNumber << endl;
        cout << "Reservasi              : " << (customer.reservation ? "Ya" : "Tidak") << endl;
        cout << "Waktu Tiba             : " << DateTime(customer.arrivalTime, "%H:%M") << endl;
        createLine(50, "-");

        cout << "Apakah Customer ini sudah datang? (y/n): ";
        char confirmation = getValidString("Masukan 'y' atau 'n': ", true)[0];

        if (confirmation == 'y' || confirmation == 'Y')
        {
            cout << "\nPelanggan sudah datang." << endl;
            return true;
        }

        cout << "\nPelanggan belum datang." << endl;

        if (attempt < MAX_ATTEMPTS)
        {
            cout << "Silakan tunggu beberapa saat." << endl;
            Sleep(5000);
            cout << "Silakan coba lagi. (Percobaan " << attempt + 1 << "/" << MAX_ATTEMPTS << ")" << endl;
            system("pause");
        }
        else
        {
            // Remove customer after 3 failed attempts
            cout << "Pelanggan tidak datang setelah " << MAX_ATTEMPTS << " percobaan." << endl;
            removeCustomerFromQueue(isReservation);
        }
    }

    return false;
}

/**
 * @brief Fungsi untuk mendudukkan pelanggan berikutnya dari antrian.
 * * Fungsi ini mengelola proses untuk mendudukkan pelanggan berikutnya.
 * Prioritas diberikan kepada pelanggan dalam antrian reservasi.
 * Jika antrian reservasi kosong, maka akan diambil pelanggan dari antrian tunggu.
 * Fungsi ini juga akan memeriksa ketersediaan meja dan status kedatangan pelanggan.
 */
void seatNextCustomer()
{
    // Periksa apakah ada pelanggan yang tersedia di kedua antrian (reservasi dan tunggu).
    if (manager.reservationQueue.empty() && manager.waitingQueue.empty())
    {
        cout << "Tidak ada pelanggan yang dapat dilayani." << endl;
        return; // Keluar dari fungsi jika tidak ada pelanggan.
    }

    // Variabel untuk menyimpan informasi pelanggan berikutnya.
    int customerId;
    bool isReservation;

    // Prioritaskan pelanggan dari antrian reservasi.
    if (!manager.reservationQueue.empty())
    {
        customerId = manager.reservationQueue[0]; // Ambil ID pelanggan pertama dari antrian reservasi.
        isReservation = true;                     // Tandai sebagai pelanggan reservasi.
    }
    else // Jika antrian reservasi kosong, ambil dari antrian tunggu.
    {
        customerId = manager.waitingQueue[0]; // Ambil ID pelanggan pertama dari antrian tunggu.
        isReservation = false;                // Tandai sebagai pelanggan non-reservasi (walk-in).
    }

    // Cari objek Customer yang sesuai berdasarkan customerId yang telah didapat.
    auto customerIter = find_if(manager.customers.begin(), manager.customers.end(),
                                [customerId](const Customer &customer)
                                {
                                    return customer.id == customerId;
                                });

    // Jika iterator menunjuk ke akhir vektor, berarti pelanggan tidak ditemukan.
    if (customerIter == manager.customers.end())
    {
        cout << "Customer not found." << endl;
        return; // Keluar dari fungsi jika data pelanggan tidak valid.
    }

    // Panggil fungsi untuk memeriksa dan memverifikasi kedatangan pelanggan.
    // Fungsi checkCustomerArrival menangani logika coba lagi (retry) jika pelanggan belum tiba.
    if (!checkCustomerArrival(*customerIter, isReservation))
    {
        return; // Keluar jika pelanggan tidak datang setelah beberapa kali percobaan.
    }

    // Setelah kedatangan pelanggan diverifikasi, proses pemilihan meja dan dudukkan pelanggan.
    processTableSeating(*customerIter, isReservation);
}