# ntfs
FS Simple Reader 🗂️
Filesystem NTFS read-only sederhana untuk OS hobby seperti BloodGOS.
Implementasi bertahap yang fokus pada pembelajaran dan kemudahan pengembangan.


✅ Saat ini sudah bisa:

Mount volume NTFS dari disk image

Baca dan parse boot sector (512 byte pertama)

Validasi signature NTFS dan boot signature 0xAA55

Tampilkan informasi dasar volume (cluster size, total sector, MFT location)

Error handling dasar untuk kasus umum



Baca file/direktori

Navigasi MFT (Master File Table)

Support file besar (non-resident data)

Integrasi dengan kernel OS

🚀 Quick Start (5 Menit)
Prerequisites
GCC compiler (Linux/Mac/WSL) atau MinGW (Windows)

Python 3 (untuk buat test image)

Terminal/command prompt

Langkah 1: Setup Project
bash
# 1. Buat folder project
mkdir simple_ntfs
cd simple_ntfs

# 2. Download semua file ke folder ini:
#    main.c, ntfs.h, ntfs.c, disk_io.c, Makefile, create_test_img.py

# 3. Build program
make

# 4. Buat disk image test
python create_test_img.py

# 5. Jalankan test!
make run
Expected Output (TAHAP 1 Berhasil):
text
================================
   NTFS SIMPLE READER - TAHAP 1
================================

Testing with disk image: test.img

[MAIN] Step 1: Mounting NTFS volume...
[NTFS] Mounting from: test.img
[NTFS] OEM ID: NTFS    
[NTFS] Bytes per cluster: 4096
[NTFS] Mount successful!
[MAIN] ✓ Mount successful!
...
📁 Struktur Project (Super Sederhana)
text
simple_ntfs/
├── src/
│   ├── main.c          # Program utama (hanya 70 baris!)
│   ├── ntfs.h          # Header file (semua definisi struct)
│   ├── ntfs.c          # Implementasi NTFS (mount/unmount/info)
│   └── disk_io.c       # Fungsi baca/tulis ke file (bukan hardware)
├── Makefile            # Build otomatis (tinggal 'make')
├── create_test_img.py  # Buat disk image NTFS dummy
└── README.md           # File ini
Penjelasan File:
ntfs.h - Hanya berisi 2 struct utama:

ntfs_boot_sector (boot sector NTFS)

ntfs_volume (state/konteks volume)

ntfs.c - Hanya 3 fungsi publik:

ntfs_mount() - Baca dan validasi boot sector

ntfs_show_info() - Tampilkan info volume

ntfs_unmount() - Cleanup sederhana

disk_io.c - Simulasi akses hardware:

Baca/tulis ke file .img (bukan hardware beneran)
main.c - Test runner standalone:

Bisa jalan tanpa kernel OS

Cocok untuk debugging cepat
