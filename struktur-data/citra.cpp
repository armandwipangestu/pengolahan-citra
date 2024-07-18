#include <iostream>
#include <cstdlib> // untuk malloc dan free
#include <cstdio>  // untuk fopen, fread, fwrite
#include <windows.h> // untuk fungsi Windows seperti HDC, HWND, dll.

using namespace std;

#define MAX_N 500       // Maksimum tinggi citra
#define MAX_M 100       // Maksimum lebar citra
typedef unsigned char **citra;

// Fungsi alokasi memori untuk citra
citra alokasi(int N, int M) {
    citra f;
    int i;

    f = (unsigned char**) malloc(N * sizeof(unsigned char*));
    if (f == NULL) {
        return NULL;
    }

    for (i = 0; i < N; i++) {
        f[i] = (unsigned char*) malloc(M * sizeof(unsigned char));
        if (f[i] == NULL) {
            // Jika alokasi gagal, dealokasi elemen yang sudah dialokasikan
            for (int j = 0; j < i; j++) {
                free(f[j]);
            }
            free(f);
            return NULL;
        }
    }
    return f;
}

// Fungsi dealokasi memori untuk citra
void dealokasi(citra f, int N) {
    for (int i = 0; i < N; i++) {
        free(f[i]);
    }
    free(f);
}

// Fungsi untuk menampilkan citra di lingkungan Windows
void WIN_tampilkan_citra(citra r, citra g, citra b, int N, int M) {
    HDC MemDC;
    HBITMAP mbitmap;
    HWND hwnd;
    COLORREF TableWarna[256];
    int i, j, palet;

    hwnd = GetConsoleWindow();  // GetActiveWindow() has been replaced with GetConsoleWindow()
    MemDC = CreateCompatibleDC(GetDC(hwnd));
    mbitmap = CreateCompatibleBitmap(GetDC(hwnd), M, N);
    SelectObject(MemDC, mbitmap);

    for (i = 0; i < 256; i++) {
        TableWarna[i] = RGB(i, i, i);
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            palet = RGB(r[i][j], g[i][j], b[i][j]);
            SetPixelV(MemDC, j, i, palet);
        }
    }

    BitBlt(GetDC(hwnd), 0, 0, M, N, MemDC, 0, 0, SRCCOPY);
    
    // Tambahkan Sleep agar window tetap terbuka
    Sleep(5000);
}

// Fungsi untuk membaca citra dari file
void baca_citra_dari_arsip(const char nama_arsip[], citra &f, int &N, int &M) {
    FILE *fp = fopen(nama_arsip, "rb");
    if (fp == NULL) {
        printf("Arsip tidak ada");
        exit(0);
    }

    fread(&N, sizeof(unsigned short int), 1, fp);
    fread(&M, sizeof(unsigned short int), 1, fp);

    f = alokasi(N, M);
    if (f == NULL) {
        printf("Memori tidak cukup");
        exit(0);
    }

    for (int i = 0; i < N; i++) {
        fread(f[i], sizeof(unsigned char), M, fp);
    }

    fclose(fp);
}

// Fungsi untuk menulis citra ke file
void tulis_citra_ke_arsip(const char nama_arsip[], citra f, int N, int M) {
    FILE *fp = fopen(nama_arsip, "wb");
    if (fp == NULL) {
        printf("Arsip tidak dapat dibuat");
        exit(0);
    }

    fwrite(&N, sizeof(unsigned short int), 1, fp);
    fwrite(&M, sizeof(unsigned short int), 1, fp);

    for (int i = 0; i < N; i++) {
        fwrite(f[i], sizeof(unsigned char), M, fp);
    }

    fclose(fp);
}

int main() {
    citra r, g, b;
    int N = MAX_N, M = MAX_M;

    r = alokasi(N, M);
    g = alokasi(N, M);
    b = alokasi(N, M);

    if (r == NULL || g == NULL || b == NULL) {
        printf("Gagal mengalokasikan memori untuk citra");
        return -1;
    }

    // Mengisi citra dengan warna acak untuk demonstrasi
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            r[i][j] = rand() % 256;
            g[i][j] = rand() % 256;
            b[i][j] = rand() % 256;
        }
    }

    // Tampilkan citra menggunakan fungsi Windows
    WIN_tampilkan_citra(r, g, b, N, M);

    dealokasi(r, N);
    dealokasi(g, N);
    dealokasi(b, N);

    return 0;
}
