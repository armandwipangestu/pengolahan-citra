#include <iostream>
#include <cstdlib> // untuk malloc dan free
#include <cstdio>  // untuk fopen, fread, fwrite
#include <windows.h> // untuk fungsi Windows seperti HDC, HWND, dll.

using namespace std;

#define MAX_N 500       // Maksimum tinggi citra
#define MAX_M 500       // Maksimum lebar citra
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

// Fungsi untuk menampilkan pixel
void setpixel(HDC MemDC, unsigned char r, unsigned char g, unsigned char b, int x, int y) {
    COLORREF color = RGB(r, g, b); // Membuat warna dari nilai r, g, b
    SetPixelV(MemDC, x, y, color); // Set pixel pada posisi x, y dengan warna
}

// Fungsi untuk menampilkan citra
void tampilkan_citra(HDC MemDC, citra r, citra g, citra b, int N, int M) {
    for (int i = 0; i < N; i++) { // Iterasi baris
        for (int j = 0; j < M; j++) { // Iterasi kolom
            setpixel(MemDC, r[i][j], g[i][j], b[i][j], j, i); // Tampilkan pixel
        }
    }
}

// Fungsi untuk menampilkan citra di lingkungan Windows
void WIN_tampilkan_citra(citra r, citra g, citra b, int N, int M) {
    HWND hwnd = GetConsoleWindow(); // Ambil handle untuk jendela konsol
    HDC hdc = GetDC(hwnd); // Ambil device context dari jendela konsol
    HDC MemDC = CreateCompatibleDC(hdc); // Buat device context yang kompatibel
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, M, N); // Buat bitmap yang kompatibel

    SelectObject(MemDC, hbmMem); // Pilih bitmap ke dalam device context

    tampilkan_citra(MemDC, r, g, b, N, M); // Tampilkan citra ke device context

    // Salin dari device context ke layar
    BitBlt(hdc, 0, 0, M, N, MemDC, 0, 0, SRCCOPY);

    Sleep(5000); // Tahan jendela selama 5 detik

    DeleteObject(hbmMem); // Hapus bitmap
    DeleteDC(MemDC); // Hapus device context
    ReleaseDC(hwnd, hdc); // Lepaskan device context
}

// Fungsi untuk membaca citra dari file
void baca_citra_dari_arsip(const char nama_arsip[], citra &f, int &N, int &M) {
    FILE *fp = fopen(nama_arsip, "rb"); // Buka file untuk membaca biner
    if (fp == NULL) { // Jika file tidak ada
        printf("Arsip tidak ada\n");
        exit(0);
    }

    fread(&N, sizeof(unsigned short int), 1, fp); // Baca tinggi citra
    fread(&M, sizeof(unsigned short int), 1, fp); // Baca lebar citra

    f = alokasi(N, M); // Alokasikan memori untuk citra
    if (f == NULL) { // Jika alokasi gagal
        printf("Memori tidak cukup\n");
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
        printf("Arsip tidak dapat dibuat\n");
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
    int N, M; // Deklarasi ukuran citra

    // Membaca citra dari file
    baca_citra_dari_arsip("gambar_r.dat", r, N, M);
    baca_citra_dari_arsip("gambar_g.dat", g, N, M);
    baca_citra_dari_arsip("gambar_b.dat", b, N, M);

    // Tampilkan citra menggunakan fungsi Windows
    WIN_tampilkan_citra(r, g, b, N, M);

    // Dealokasi memori citra
    dealokasi(r, N);
    dealokasi(g, N);
    dealokasi(b, N);

    return 0; // Selesai
}
