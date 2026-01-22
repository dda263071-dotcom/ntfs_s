/**
 * Disk I/O sederhana - baca/tulis ke file image
 * Sangat sederhana untuk awal pembelajaran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ntfs.h"

// Global file handle untuk simulasi disk
static FILE *disk_image = NULL;

/**
 * Buka disk image untuk dibaca
 */
int disk_open(const char *filename) {
    disk_image = fopen(filename, "rb");
    if (!disk_image) {
        printf("Error: Cannot open disk image %s\n", filename);
        return -1;
    }
    return 0;
}

/**
 * Baca sejumlah sector dari disk
 */
int disk_read_sector(uint64_t sector, void *buffer, uint32_t count) {
    if (!disk_image) {
        printf("Error: Disk not opened\n");
        return -1;
    }
    
    // Cari posisi sector (1 sector = 512 bytes)
    fseek(disk_image, sector * 512, SEEK_SET);
    
    // Baca data
    size_t bytes_read = fread(buffer, 512, count, disk_image);
    
    return (bytes_read == count) ? 0 : -1;
}

/**
 * Tutup disk image
 */
void disk_close(void) {
    if (disk_image) {
        fclose(disk_image);
        disk_image = NULL;
    }
}

/**
 * Baca bytes secara langsung (untuk boot sector)
 */
int disk_read_bytes(uint64_t offset, void *buffer, uint32_t size) {
    if (!disk_image) return -1;
    
    fseek(disk_image, offset, SEEK_SET);
    size_t read = fread(buffer, 1, size, disk_image);
    
    return (read == size) ? 0 : -1;
}
