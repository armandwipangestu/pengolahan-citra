#include <iostream>

using namespace std;

#define N 500       /* Maksimum tinggi citra */
#define M 100       /* Maksimum lebar citra */
unsigned char f[N][M];
unsigned char *a;
// a = (unsigned char*) malloc(N * sizeof(unsigned char));
typedef unsigned char **citra;
citra f;

citra alokasi(int N, int M) {
    // Mengalokasikan memori untuk citra f yang berukuran N x M pixel.
    citra f;
    int i;

    f = (unsigned char**) malloc(N * sizeof(unsigned char*));
    
    if (f == NULL) {
        return(NULL);
    }
    
    for (i = 0; i < N; i++) {
        f[i] = (unsigned char*) malloc(M * sizeof(unsigned char));
        
        if (f[i] == NULL) {
            // Memori habis, dealokasi semua elemen baris matriks
            dealokasi(f, N);
            return(NULL);
        }
    }

    return f;
}

void dealokasi(citra f, int N) {
    // Dealokasi memori dari citra f yang mempunyai N baris pixel.
    int i;

    for (i = 0; i < N; i++) {
        free(f[i]); /* Bebaskan memori semua elemen pada baris i */
    }
    free(f);
}

void **alokasi(int N, int M, int UkuranElemen) {
    // Mengalokasikan memori untuk matriks N x M. Setiap elemen matriks berukuran UkuranElemen byte
    int i;
    void **larik = (void**) xalloc(N * sizeof(void *)); /* buat array N elemen */
    for (i = 0; i < N; i++) {
        larik[i] = (void*) xalloc(M * UkuranElemen);
    }
    return larik;
}

void *xalloc(unsigned ukuran) {
    // Mengalokasikan memori dan memeriksa apakah alokasi memori berhasil
    void *p = malloc(ukuran);
    if (p == NULL) {
        printf("Memori tidak cukup untuk alokasi matriks");
        exit(0);
    }

    return p;
}

Image = (unsigned char**) alokasi(N, M, sizeof(unsigned char));
Mat = (float**) alokasi(N, M, sizeof(float));

setpixel(unsigned char r, unsigned char g, unsigned char b, int i, int j);
/* Menampilkan pixel dengan komponen rgb pada koordinat i, j */
/* Perhatikan: setpixel bukan standard C */

void tampilkan_citra(citra r, citra g, citra b, int N, int M) {
    // Menampilkan citra yang berukuran N x M pixel ke layar.
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            // setpixel(r[i][j], g[i][j], b[i][j], j, i);
            setpixel(f[i][j], f[i][j], f[i][j], j, i);
        }
    }
}

void WIN_tampilkan_citra(citra Image, int N, int M) {
    // Menampilkan citra Image yang berukuran N x M di lingkungan windows
    HDC         MemDC;              /* Handle ke memory device context */
    HBITMAP     mbitmap;            /* Handle ke citra */
    HWND        hwnd;               /* Handle ke window */
    COLORREF    TableWarna[256];    /* Tabel warna (palet) */
    int i, j, palet;

    hwnd = GetActiveWindow();
    MemDC = CreateCompatibleDC(GetDC(hwnd));
    mbitmap = CreateCompatibleBitmap(GetDC(hwnd), M, N);
    SelectObject(MemDC, mbitmap);

    // Definisikan palet
    for (i = 0; i < 256; i++) {
        TableWarna[i] = GetNearestColor(MemDC, RGB(i, i, i));
    }

    // Isikan pixel ke memori device (layar)
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            // palet = Image[i][j];
            // SetPixelV(MemDC, j, i, TabelWarna[palet]);
            palet = GetNearestColor(MemDC, RGB(r[i][j], g[i][j], b[i][j]));
            SetPixelV(MemDC, j, i, palet);
        }
    }

    // Tembakkan citra ke layar
    BitBlt(GetDC(hwnd), 0, 0, M, N, MemDC, 0, 0, SRCCOPY);
}

void baca_citra_dari_arsip(char nama_arsip[], citra f) {
    // Membaca citra dari file citra mentah. Citra hasil pembacaan disimpan di dalam matriks f.
    FILE *fp;
    int i, j;
    unsigned short int N, M;

    if ((fp = fopen(nama_arsip, "rb")) == NULL) {
        printf("Arsip tidak ada");
        exit(0);
    }

    fread(&N, sizeof(unsigned short int), 1, fp); /* baca tinggi citra */
    fread(&M, sizeof(unsigned short int), 1, fp); /* baca lebar citra */

    f = alokasi(N, M); /* Alokasi memori matriks untuk citra f */
    if (f == NULL) {
        printf("Memori tidak cukup");
        exit(0);
    }

    // Baca data citra baris demi baris
    for (int 0; i < N; i++) {
        // Baca data citra baris ke-i
        fread(f[i], sizeof(citra unsigned char), M, fp);
    }

    close(fp);
}

void tulis_ctira_ke_arsip(char nama_arsip[], citra f) {
    // Menulis citra f ke dalam arsip nama_arsip
    FILE *fp;
    int i, j;
    unsigned short int N, M;

    if ((fp = fopen(nama_arsip, "wb")) == NULL) {
        printf("Arsip tidak dapat dibuat");
        exit(0);
    }

    fwrite(N, sizeof(unsigned short int), 1, fp); /* tulis tinggi citra */
    fwrite(M, sizeof(unsigned short int), 1, fp); /* tulis lebar citra */

    // Baca data citra baris demi baris
    for (i = 0; i < N; i++) {
        // Tulis data citra baris ke-i
        fwrite(f[i], sizeof(unsigned char), M, fp);
    }

    close(fp);
}

r = alokasi(N, M);
g = alokasi(N, M);
b = alokasi(N, M);