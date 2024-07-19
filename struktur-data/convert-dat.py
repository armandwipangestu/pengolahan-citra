import cv2
import numpy as np

# Baca gambar menggunakan OpenCV
img = cv2.imread('../assets/drawing-function.jpg')

# Ubah ukuran gambar sesuai kebutuhan (maksimum N x M)
N = 500
M = 500
img_resized = cv2.resize(img, (M, N))

# Pisahkan menjadi tiga kanal warna
r, g, b = cv2.split(img_resized)

# Fungsi untuk menyimpan setiap kanal ke file .dat
def save_channel_to_dat(channel, filename):
    with open(filename, 'wb') as f:
        # Tulis ukuran N dan M sebagai unsigned short int
        f.write(np.uint16(N).tobytes())
        f.write(np.uint16(M).tobytes())
        # Tulis data channel sebagai unsigned char
        f.write(channel.tobytes())

# Simpan setiap kanal ke file .dat
save_channel_to_dat(r, 'gambar_r.dat')
save_channel_to_dat(g, 'gambar_g.dat')
save_channel_to_dat(b, 'gambar_b.dat')
