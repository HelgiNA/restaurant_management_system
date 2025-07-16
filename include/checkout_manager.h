#ifndef CHECKOUT_MANAGER_H
#define CHECKOUT_MANAGER_H

#include "restaurant_system.h"

// ==================================================
// DEKLARASI FUNGSI PROSES CHECKOUT
// ==================================================

/**
 * @brief Menghitung total tagihan untuk sebuah meja.
 * @param tableId ID meja yang akan dihitung.
 * @return Total harga dari semua pesanan di meja tersebut.
 */
double calculateBill(int tableId);

/**
 * @brief Mencetak struk pembayaran ke sebuah file teks.
 * @param tableId ID meja yang checkout.
 * @param totalBill Total tagihan yang harus dibayar.
 */
void printReceipt(int tableId, double totalBill);

/**
 * @brief Memproses pembayaran dan menyelesaikan sesi pelanggan.
 * @param tableId ID meja yang akan checkout.
 */
void processPayment(int tableId);

#endif // CHECKOUT_MANAGER_H
