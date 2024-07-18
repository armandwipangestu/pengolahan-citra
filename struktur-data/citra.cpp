#include <iostream>
#include <cstdlib> // untuk malloc dan free
#include <cstdio>  // untuk fopen, fread, fwrite
#include <windows.h> // untuk fungsi Windows seperti HDC, HWND, dll.

using namespace std;

#define MAX_N 500       // Maksimum tinggi citra
#define MAX_M 100       // Maksimum lebar citra
typedef unsigned char **citra; // Tipe data citra sebagai matriks 2D dari unsigned char

// Fungsi alokasi memori untuk citra
citra alokasi(int N, int M) {
    citra f; // Deklarasi variabel citra
    int i;

    // Alokasi memori untuk baris
    f = (unsigned char**) malloc(N * sizeof(unsigned char*));
    if (f == NULL) { // Jika alokasi gagal
        return NULL;
    }

    // Alokasi memori untuk setiap kolom
    for (i = 0; i < N; i++) {
        f[i] = (unsigned char*) malloc(M * sizeof(unsigned char));
        if (f[i] == NULL) { // Jika alokasi gagal
            // Dealokasi elemen yang sudah dialokasikan
            for (int j = 0; j < i; j++) {
                free(f[j]);
            }
            free(f);
            return NULL;
        }
    }
    return f; // Kembalikan pointer ke citra
}

// Fungsi dealokasi memori untuk citra
void dealokasi(citra f, int N) {
    for (int i = 0; i < N; i++) {
        free(f[i]); // Bebaskan memori setiap baris
    }
    free(f); // Bebaskan memori utama
}

// Fungsi untuk menampilkan pixel (simulasi, tidak standar C)
void setpixel(unsigned char r, unsigned char g, unsigned char b, int i, int j) {
    // Ini adalah fungsi placeholder, implementasi tergantung platform grafik
    // Misalnya, menggunakan OpenCV:
    // cv::Mat img = cv::Mat::zeros(MAX_N, MAX_M, CV_8UC3);
    // img.at<cv::Vec3b>(i, j) = cv::Vec3b(b, g, r);
    // cv::imshow("Image", img);
}

// Fungsi untuk menampilkan citra
void tampilkan_citra(citra r, citra g, citra b, int N, int M) {
    for (int i = 0; i < N; i++) { // Iterasi baris
        for (int j = 0; j < M; j++) { // Iterasi kolom
            setpixel(r[i][j], g[i][j], b[i][j], i, j); // Tampilkan pixel
        }
    }
}

// Fungsi untuk menampilkan citra di lingkungan Windows
void WIN_tampilkan_citra(citra r, citra g, citra b, int N, int M) {
    HDC MemDC; // Deklarasi perangkat kontekstual memori
    HBITMAP mbitmap; // Deklarasi bitmap
    HWND hwnd; // Deklarasi handle untuk window
    COLORREF TableWarna[256]; // Deklarasi tabel warna
    int i, j, palet;

    hwnd = GetConsoleWindow(); // Ambil handle untuk jendela konsol
    MemDC = CreateCompatibleDC(GetDC(hwnd)); // Buat perangkat kontekstual yang kompatibel
    mbitmap = CreateCompatibleBitmap(GetDC(hwnd), M, N); // Buat bitmap yang kompatibel
    SelectObject(MemDC, mbitmap); // Pilih bitmap ke dalam perangkat kontekstuak

    for (i = 0; i < 256; i++) {
        TableWarna[i] = RGB(i, i, i); // Inisialisasi tabel warna grayscale
    }

    for (i = 0; i < N; i++) { // Iterasi baris
        for (j = 0; j < M; j++) { // Iterasi kolom
            palet = RGB(r[i][j], g[i][j], b[i][j]); // Dapatkan nilai warna dari citra
            SetPixelV(MemDC, j, i, palet); // Set pixel di perangkat kontekstuak
        }
    }

    BitBlt(GetDC(hwnd), 0, 0, M, N, MemDC, 0, 0, SRCCOPY); // Salin dari perangkat kontekstuak ke layar

    Sleep(5000); // Tahan jendela selama 5 detik
}

// Fungsi untuk membaca citra dari file
void baca_citra_dari_arsip(const char nama_arsip[], citra &f, int &N, int &M) {
    FILE *fp = fopen(nama_arsip, "rb"); // Buka file untuk membaca biner
    if (fp == NULL) { // Jika file tidak ada
        printf("Arsip tidak ada");
        exit(0);
    }

    fread(&N, sizeof(unsigned short int), 1, fp); // Baca tinggi citra
    fread(&M, sizeof(unsigned short int), 1, fp); // Baca lebar citra

    f = alokasi(N, M); // Alokasikan memori untuk citra
    if (f == NULL) { // Jika alokasi gagal
        printf("Memori tidak cukup");
        exit(0);
    }

    for (int i = 0; i < N; i++) {
        fread(f[i], sizeof(unsigned char), M, fp); // Baca setiap baris dari file
    }

    fclose(fp); // Tutup file
}

// Fungsi untuk menulis citra ke file
void tulis_citra_ke_arsip(const char nama_arsip[], citra f, int N, int M) {
    FILE *fp = fopen(nama_arsip, "wb"); // Buka file untuk menulis biner
    if (fp == NULL) { // Jika file tidak dapat dibuat
        printf("Arsip tidak dapat dibuat");
        exit(0);
    }

    fwrite(&N, sizeof(unsigned short int), 1, fp); // Tulis tinggi citra
    fwrite(&M, sizeof(unsigned short int), 1, fp); // Tulis lebar citra

    for (int i = 0; i < N; i++) {
        fwrite(f[i], sizeof(unsigned char), M, fp); // Tulis setiap baris ke file
    }

    fclose(fp); // Tutup file
}

// Fungsi utama
int main() {
    citra r, g, b; // Deklarasi tiga citra untuk warna merah, hijau, dan biru
    int N = MAX_N, M = MAX_M; // Inisialisasi ukuran citra

    r = alokasi(N, M); // Alokasikan memori untuk citra merah
    g = alokasi(N, M); // Alokasikan memori untuk citra hijau
    b = alokasi(N, M); // Alokasikan memori untuk citra biru

    if (r == NULL || g == NULL || b == NULL) { // Jika alokasi gagal
        printf("Gagal mengalokasikan memori untuk citra");
        return -1;
    }

    // Mengisi citra dengan warna acak untuk demonstrasi
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            r[i][j] = rand() % 256; // Isi citra merah dengan nilai acak
            g[i][j] = rand() % 256; // Isi citra hijau dengan nilai acak
            b[i][j] = rand() % 256; // Isi citra biru dengan nilai acak
        }
    }

    // Tampilkan citra menggunakan fungsi Windows
    WIN_tampilkan_citra(r, g, b, N, M);

    dealokasi(r, N); // Dealokasi memori citra merah
    dealokasi(g, N); // Dealokasi memori citra hijau
    dealokasi(b, N); // Dealokasi memori citra biru

    return 0; // Selesai
}
