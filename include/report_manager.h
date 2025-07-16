#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

#include "restaurant_system.h"

// ==================================================
// DEKLARASI FUNGSI MANAJEMEN LAPORAN
// ==================================================

/**
 * @brief Membuat laporan pendapatan dan statistik harian.
 */
void generateDailyReport();

/**
 * @brief Membuat laporan pendapatan bulanan.
 */
void generateMonthlyReport();

/**
 * @brief Membuat laporan performa menu (item terlaris).
 */
void generateMenuPerformanceReport();

/**
 * @brief Fungsi utama untuk menampilkan antarmuka menu laporan.
 */
void reportManagement();

#endif // REPORT_MANAGER_H
