#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "restaurant_system.h"

// ==================================================
// DEKLARASI FUNGSI MANAJEMEN MENU
// ==================================================

/**
 * @brief Menginisialisasi daftar menu dengan beberapa data default.
 * * Fungsi ini akan dipanggil sekali saat program dimulai untuk mengisi
 * data menu jika belum ada.
 */
void initializeDefaultMenu();

/**
 * @brief Menampilkan semua item menu, dikelompokkan berdasarkan kategori.
 * * Fungsi ini akan mencetak daftar menu yang rapi ke konsol.
 */
void displayMenu();

/**
 * @brief Menambahkan item menu baru.
 * * (Akan diimplementasikan pada tahap selanjutnya)
 */
void addMenuItem();

/**
 * @brief Mengubah status ketersediaan item menu (Tersedia/Habis).
 * * (Akan diimplementasikan pada tahap selanjutnya)
 */
void updateItemAvailability();

/**
 * @brief Fungsi utama untuk menampilkan antarmuka manajemen menu.
 * * Menangani input pengguna untuk memilih aksi terkait menu.
 */
void menuManagement();

#endif // MENU_MANAGER_H
