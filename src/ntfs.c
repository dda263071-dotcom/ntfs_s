/**
 * Implementasi NTFS Sederhana - READ ONLY
 * TAHAP 1: Hanya bisa mount dan baca informasi dasar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ntfs.h"

// Deklarasi fungsi disk I/O (implementasi di disk_io.c)
int disk_open(const char *filename);
int disk_read_bytes(uint64_t offset, void *buffer, uint32_t size);
void disk_close(void);

/**
 * Mount volume NTFS
 */
int ntfs_mount(struct ntfs_volume *vol, const char *disk_path) {
    printf("[NTFS] Mounting from: %s\n", disk_path);
    
    // 1. Buka disk image
    if (disk_open(disk_path) != 0) {
        printf("[NTFS] Error: Cannot open disk image\n");
        return -1;
    }
    
    // 2. Baca boot sector (512 bytes pertama)
    memset(vol, 0, sizeof(struct ntfs_volume));
    
    if (disk_read_bytes(0, &vol->boot, sizeof(struct ntfs_boot_sector)) != 0) {
        printf("[NTFS] Error: Cannot read boot sector\n");
        disk_close();
        return -2;
    }
    
    // 3. Validasi signature 0xAA55
    if (vol->boot.signature != 0xAA55) {
        printf("[NTFS] Error: Invalid boot signature (got 0x%04X, expected 0xAA55)\n", 
               vol->boot.signature);
        disk_close();
        return -3;
    }
    
    // 4. Cek OEM ID untuk konfirmasi NTFS (tidak wajib, tapi baik)
    printf("[NTFS] OEM ID: %.8s\n", vol->boot.oem_id);
    
    // 5. Hitung bytes per cluster
    vol->bytes_per_cluster = vol->boot.bytes_per_sector * vol->boot.sectors_per_cluster;
    printf("[NTFS] Bytes per cluster: %u\n", vol->bytes_per_cluster);
    
    // 6. Set flag mounted
    vol->is_mounted = 1;
    
    // 7. Tutup disk (untuk sekarang, nanti bisa dibuka lagi)
    disk_close();
    
    printf("[NTFS] Mount successful!\n");
    return 0;
}

/**
 * Tampilkan informasi detail volume
 */
void ntfs_show_info(struct ntfs_volume *vol) {
    if (!vol->is_mounted) {
        printf("Volume not mounted\n");
        return;
    }
    
    printf("\n=== NTFS VOLUME INFORMATION ===\n");
    printf("OEM ID:              %.8s\n", vol->boot.oem_id);
    printf("Bytes per Sector:    %u\n", vol->boot.bytes_per_sector);
    printf("Sectors per Cluster: %u\n", vol->boot.sectors_per_cluster);
    printf("Bytes per Cluster:   %u (%u KB)\n", 
           vol->bytes_per_cluster, vol->bytes_per_cluster / 1024);
    printf("Total Sectors:       %llu\n", vol->boot.total_sectors);
    printf("Total Size:          %llu MB\n", 
           (vol->boot.total_sectors * vol->boot.bytes_per_sector) / (1024 * 1024));
    
    // Decode MFT cluster size (special encoding)
    uint32_t mft_cluster_size;
    if (vol->boot.clusters_per_mft < 0xF0) {
        mft_cluster_size = vol->boot.clusters_per_mft * vol->bytes_per_cluster;
    } else {
        // Negative values mean power of 2
        uint32_t shift = 256 - vol->boot.clusters_per_mft;
        mft_cluster_size = 1 << shift;  // 2^shift
    }
    printf("MFT Record Size:     %u bytes\n", mft_cluster_size);
    
    printf("MFT Start Cluster:   %llu\n", vol->boot.mft_start_lcn);
    printf("MFT2 Start Cluster:  %llu\n", vol->boot.mft2_start_lcn);
    printf("Volume Serial:       %llX\n", vol->boot.volume_serial);
    printf("===============================\n");
}

/**
 * Unmount volume
 */
void ntfs_unmount(struct ntfs_volume *vol) {
    if (vol->is_mounted) {
        vol->is_mounted = 0;
        printf("[NTFS] Volume unmounted\n");
    }
}
