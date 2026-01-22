/**
 * NTFS Simple Header File
 * Hanya definisi paling dasar untuk NTFS read-only
 */

#ifndef NTFS_SIMPLE_H
#define NTFS_SIMPLE_H

#include <stdint.h>
#include <stddef.h>

// ======================
// STRUKTUR DATA NTFS
// ======================

#pragma pack(push, 1)  // Pastikan tidak ada padding

/**
 * Boot Sector NTFS (512 bytes pertama)
 * Hanya field yang paling penting untuk read-only
 */
struct ntfs_boot_sector {
    // BIOS Parameter Block (BPB)
    uint8_t  jump_code[3];          // Jump instruction (E9 XX XX)
    uint8_t  oem_id[8];             // "NTFS    " atau "MSDOS5.0"
    
    // BIOS Parameter Block (lanjutan)
    uint16_t bytes_per_sector;      // Biasanya 512
    uint8_t  sectors_per_cluster;   // 1, 2, 4, 8, 16, 32, 64, 128
    uint16_t reserved_sectors;      // Selalu 0 untuk NTFS
    uint8_t  unused1[5];            // Tidak dipakai NTFS
    uint8_t  media_descriptor;      // F8 untuk hard disk
    uint16_t unused2;               // Selalu 0
    uint16_t sectors_per_track;     // Untuk geometri disk
    uint16_t number_of_heads;       // Untuk geometri disk
    uint32_t hidden_sectors;        // Sectors sebelum partisi
    uint32_t unused3;               // Tidak dipakai NTFS
    
    // Extended BIOS Parameter Block
    uint32_t unused4;               // Tidak dipakai NTFS
    uint64_t total_sectors;         // Total sectors dalam volume
    uint64_t mft_start_lcn;         // Cluster pertama MFT (Logical Cluster Number)
    uint64_t mft2_start_lcn;        // Backup MFT
    uint32_t clusters_per_mft;      // Biasanya 0xF6 = 246? Tidak, decode khusus
    uint32_t clusters_per_index;    // Cluster per index buffer
    uint64_t volume_serial;         // Serial number volume
    uint32_t checksum;              // Checksum
    
    // Bootstrap code dan signature
    uint8_t  bootstrap[426];        // Boot code
    uint16_t signature;             // 0xAA55
};

#pragma pack(pop)  // Kembali ke packing normal

/**
 * Volume Context - simpan state NTFS
 */
struct ntfs_volume {
    struct ntfs_boot_sector boot;   // Boot sector yang sudah dibaca
    uint32_t bytes_per_cluster;     // bytes_per_sector * sectors_per_cluster
    uint32_t is_mounted;            // Flag: 1 = mounted, 0 = tidak
    char     volume_name[32];       // Nama volume (opsional)
};

// ======================
// FUNGSI PUBLIK
// ======================

/**
 * Mount volume NTFS dari disk image
 * Return: 0 jika sukses, negatif jika error
 */
int ntfs_mount(struct ntfs_volume *vol, const char *disk_path);

/**
 * Tampilkan informasi volume NTFS
 */
void ntfs_show_info(struct ntfs_volume *vol);

/**
 * Unmount volume NTFS
 */
void ntfs_unmount(struct ntfs_volume *vol);

#endif // NTFS_SIMPLE_H
