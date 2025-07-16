#include "../include/restaurant_system.h"

// Queue management functions implementation
int isEmpty(bool reservation)
{
    if (queue.tail == -1)
    {
        queue.head = -1;
        return 1; // Queue is empty
    }
    return reservation ? (manager.reservationQueue.empty() ? 1 : 0)
                       : (manager.waitingQueue.empty() ? 1 : 0);
}

int isFull()
{
    if (queue.tail == 30 - 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int generateID()
{
    // Generate random ID for customers between 1000-9999
    srand(time(0));                // Seed for random number generator
    int id = rand() % 9000 + 1000; // ID between 1000-9999
    // Check if ID already exists in the queue
    for (const auto &customer : manager.customers)
    {
        if (customer.id == id)
        {
            return generateID(); // If ID already exists, generate a new ID
        }
    }
    return id;
}

void addToQueue(string name, int partySize, string phoneNumber, bool reservation, time_t arrivalTime)
{
    // 1. Buat objek Customer baru secara eksplisit
    Customer newCustomer;
    newCustomer.id = generateID(); // Generate ID unik untuk pelanggan
    newCustomer.name = name;
    newCustomer.partySize = partySize;
    newCustomer.phoneNumber = phoneNumber;
    newCustomer.arrivalTime = arrivalTime;
    newCustomer.seatedTime = 0; // Waktu duduk diinisialisasi ke 0
    newCustomer.reservation = reservation;
    // newCustomer.paymentStatus akan menggunakan nilai default "Belum Bayar"

    // 2. Masukkan objek yang sudah jadi ke dalam vektor customers
    manager.customers.push_back(newCustomer);

    // 3. Tambahkan ID pelanggan ke antrian yang sesuai
    if (reservation)
    {
        manager.reservationQueue.push_back(newCustomer.id);
    }
    else
    {
        manager.waitingQueue.push_back(newCustomer.id);
    }

    // Logika queue.head dan queue.tail mungkin sisa dari implementasi lama
    // dan tidak terlalu relevan untuk std::vector, namun kita biarkan untuk saat ini.
    if (queue.head == -1)
    {
        queue.head = 0;
    }
    queue.tail++; // Indikator jumlah total pelanggan di antrian
}

// ... (sisa fungsi di file queue_manager.cpp)

void dequeue()
{
    if (!isEmpty(false))
    {
        // Implementation can be expanded as needed
        cout << "Customer served." << endl;
    }
    else
    {
        cout << "Data Kosong" << endl;
    }
}

void clearQueue()
{
    queue.head = -1;
    queue.tail = -1;
    manager.waitingQueue.clear();
    manager.reservationQueue.clear();
    manager.customers.clear(); // Hapus juga data pelanggan jika antrian dibersihkan total
    cout << "Semua data antrian terhapus.\n";
}

// Helper function to remove customer from queue
void removeCustomerFromQueue(bool isReservation)
{
    if (isReservation)
    {
        if (!manager.reservationQueue.empty())
        {
            cout << "Menghapus dari antrian reservasi." << endl;
            manager.reservationQueue.erase(manager.reservationQueue.begin());
        }
    }
    else
    {
        if (!manager.waitingQueue.empty())
        {
            cout << "Menghapus dari antrian tunggu." << endl;
            manager.waitingQueue.erase(manager.waitingQueue.begin());
        }
    }
}

void queueManagement()
{
    int choice = 0;
    do
    {
        system("cls");
        displayHeading("MANAJEMEN ANTRIAN RESTORAN", 50, '=', true);

        cout << "\nMENU MANAJEMEN ANTRIAN:" << endl;
        cout << "1. Tambah Pelanggan ke Antrian" << endl;
        cout << "2. Tambah Pelanggan ke Reservasi" << endl;
        cout << "3. Lihat Antrian" << endl;
        cout << "4. Tempatkan Pelanggan Berikutnya" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        createLine(50, "-");
        cout << "Masukan Pilihan Anda : ";
        choice = getValidInput(0, 4);

        switch (choice)
        {
        case 1:
            addCustomerToQueue(false); // Memanggil addCustomerToQueue tanpa reservasi
            break;
        case 2:
            addCustomerToQueue(true);
            break;
        case 3:
            viewQueue();
            break;
        case 4:
            seatNextCustomer();
            break;
        case 0:
            cout << "Kembali ke Menu Utama." << endl;
            break;
        default:
            cout << "Pilihan tidak valid. Silakan pilih antara 0-4." << endl;
            break;
        }
        system("pause");
    } while (choice != 0);
}
