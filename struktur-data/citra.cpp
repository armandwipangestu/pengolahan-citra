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

// Fungsi untuk mengalokasikan memori dengan pemeriksaan kesalahan
void *xalloc(unsigned ukuran) {
    void *p = malloc(ukuran);
    if (p == NULL) {
        printf("Memori tidak cukup untuk alokasi matriks");
        exit(0);
    }
    return p;
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
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            setpixel(r[i][j], g[i][j], b[i][j], i, j);
        }
    }
}

// Fungsi untuk menampilkan citra di lingkungan Windows
void WIN_tampilkan_citra(citra r, citra g, citra b, int N, int M) {
    HDC MemDC;
    HBITMAP mbitmap;
    HWND hwnd;
    COLORREF TableWarna[256];
    int i, j, palet;

    hwnd = GetActiveWindow();
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

    // Contoh penggunaan
    tampilkan_citra(r, g, b, N, M);

    dealokasi(r, N);
    dealokasi(g, N);
    dealokasi(b, N);

    return 0;
}
