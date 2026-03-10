#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>   // Untuk membuat nomor tiket unik

// Definisi struct untuk rute perjalanan
typedef struct {
    char nama[60]; // Perluas ukuran nama rute
    int harga;
    char kode_rute[5]; // Contoh: SBB, BLG, PRS, PST, TRM
} Rute;

// Definisi struct untuk pemesanan tiket
typedef struct {
    char no_tiket[20];
    char tanggal_berangkat[11]; // Format YYYY-MM-DD
    char jam_berangkat[6];    // Format HH:MM
    Rute rute_terpilih;
    int jumlah_kursi;
    char nama_pemesan[50];
    char nomor_hp[15];
    char metode_pembayaran[20]; // Online/Loket
    int total_harga;
} Pemesanan;

// Array rute yang tersedia (diperbarui sesuai prompt terbaru)
Rute daftar_rute[] = {
    {"Tarutung - Siborong-borong", 15000, "SBB"}, 
    {"Siborong-borong - Tarutung", 15000, "STT"}, // Kode unik untuk rute balik
    {"Tarutung - Balige", 30000, "BLG"},
    {"Balige - Tarutung", 30000, "BTT"}, // Kode unik untuk rute balik
    {"Tarutung - Porsea", 40000, "PRS"},
    {"Porsea - Tarutung", 40000, "PTT"}, // Kode unik untuk rute balik
    {"Tarutung - Parapat - Siantar", 65000, "PST"},
    {"Siantar - Parapat - Tarutung", 65000, "SPT"}, // Kode unik untuk rute balik
    {"Tarutung - Tebing Tinggi - Lubuk Pakam - Medan", 100000, "TRM"}, 
    {"Medan - Lubuk Pakam - Tebing Tinggi - Tarutung", 100000, "MTT"} // Kode unik untuk rute balik
};
int jumlah_rute = sizeof(daftar_rute) / sizeof(daftar_rute[0]);

// Fungsi untuk membersihkan buffer input (alternatif fflush(stdin) yang tidak standar)
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi untuk mendapatkan input string dengan spasi
void getInputString(char *buffer, int max_length) {
    fgets(buffer, max_length, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Menghilangkan newline character
}

// Fungsi untuk mendapatkan input integer yang valid
int getValidIntegerInput(const char *prompt) {
    char buffer[100];
    int value;
    int validInput = 0;

    do {
        printf("%s", prompt);
        getInputString(buffer, sizeof(buffer));
        if (sscanf(buffer, "%d", &value) == 1) { // Memeriksa apakah berhasil membaca integer
            validInput = 1;
        } else {
            printf("Input tidak valid. Mohon masukkan angka.\n");
        }
    } while (!validInput);
    return value;
}

// Fungsi untuk menghasilkan nomor tiket
void generateNomorTiket(char *buffer, const char *kode_rute) {
    time_t timer;
    struct tm *tm_info;
    char tahun[5];
    static int urutan_tiket = 0; // Menggunakan static untuk nomor urut yang bertambah

    time(&timer);
    tm_info = localtime(&timer);
    strftime(tahun, 5, "%Y", tm_info);

    urutan_tiket++; // Tambah nomor urut setiap kali tiket dibuat
    sprintf(buffer, "KBT-%s-%s-%03d", kode_rute, tahun, urutan_tiket);
}

// Fungsi untuk menampilkan daftar rute dan harga
void tampilkanDaftarRute() {
    printf("\n--- DAFTAR RUTE & HARGA KBT ---\n");
    for (int i = 0; i < jumlah_rute; i++) {
        printf("%d. %s: Rp %d\n", i + 1, daftar_rute[i].nama, daftar_rute[i].harga);
    }
    printf("--------------------------------\n");
}

// Fungsi utama pemesanan tiket
void pesanTiket(Pemesanan *tiket) {
    int pilihan_rute;

    printf("\n--- FORM PEMESANAN TIKET KBT ---\n");

    // 1. Pilih tanggal dan jam keberangkatan
    printf("Masukkan Tanggal Keberangkatan (YYYY-MM-DD): ");
    getInputString(tiket->tanggal_berangkat, sizeof(tiket->tanggal_berangkat));
    printf("Masukkan Jam Keberangkatan (HH:MM): ");
    getInputString(tiket->jam_berangkat, sizeof(tiket->jam_berangkat)); 

    // 2. Pilih rute perjalanan
    tampilkanDaftarRute();
    pilihan_rute = getValidIntegerInput("Pilih nomor rute: ");

    while (pilihan_rute < 1 || pilihan_rute > jumlah_rute) {
        printf("Pilihan rute tidak valid. ");
        pilihan_rute = getValidIntegerInput("Mohon masukkan antara 1 dan %d: ");
    }
    tiket->rute_terpilih = daftar_rute[pilihan_rute - 1]; // Simpan rute yang dipilih

    // 3. Pilih jumlah kursi
    tiket->jumlah_kursi = getValidIntegerInput("Masukkan Jumlah Kursi yang Dipesan: ");
    while (tiket->jumlah_kursi <= 0) {
        printf("Jumlah kursi tidak valid. ");
        tiket->jumlah_kursi = getValidIntegerInput("Mohon masukkan angka positif: ");
    }

    // Hitung total harga
    tiket->total_harga = tiket->rute_terpilih.harga * tiket->jumlah_kursi;
    printf("Harga tiket per kursi: Rp %d\n", tiket->rute_terpilih.harga);
    printf("Total harga pesanan Anda: Rp %d\n", tiket->total_harga);

    // 4. Nama
    printf("Masukkan Nama Pemesan: ");
    getInputString(tiket->nama_pemesan, sizeof(tiket->nama_pemesan));

    // 5. Nomor HP
    printf("Masukkan Nomor HP Pemesan: ");
    getInputString(tiket->nomor_hp, sizeof(tiket->nomor_hp));

    // 6. Sistem pembayaran
    printf("Pilih Metode Pembayaran (Online/Loket): ");
    getInputString(tiket->metode_pembayaran, sizeof(tiket->metode_pembayaran));

    // Generate nomor tiket
    generateNomorTiket(tiket->no_tiket, tiket->rute_terpilih.kode_rute);

    printf("\nPemesanan berhasil dicatat!\n");
}

// Fungsi untuk mencetak tiket
void cetakTiket(const Pemesanan *tiket) {
    printf("\n============ TIKET KBT ============\n");
    printf("Nomor Tiket    : %s\n", tiket->no_tiket);
    printf("Nama Pemesan   : %s\n", tiket->nama_pemesan);
    printf("Nomor HP       : %s\n", tiket->nomor_hp);
    printf("Tanggal/Jam    : %s %s\n", tiket->tanggal_berangkat, tiket->jam_berangkat);
    printf("Rute Perjalanan: %s\n", tiket->rute_terpilih.nama);
    printf("Jumlah Kursi   : %d\n", tiket->jumlah_kursi);
    printf("Harga per Kursi: Rp %d\n", tiket->rute_terpilih.harga);
    printf("Total Pembayaran: Rp %d\n", tiket->total_harga);
    printf("Metode Bayar   : %s\n", tiket->metode_pembayaran);
    printf("===================================\n");
}

int main() {
    Pemesanan pemesanan_baru; // Buat instance pemesanan

    // Untuk memastikan generator nomor tiket dimulai dari awal jika program dijalankan ulang
    // (Dalam aplikasi nyata, ini akan diambil dari database atau file)
    srand(time(NULL)); 

    pesanTiket(&pemesanan_baru); // Panggil fungsi pemesanan
    cetakTiket(&pemesanan_baru);  // Panggil fungsi cetak tiket

    printf("\nTerima kasih telah menggunakan jasa KBT!\n");

    return 0;
}
