/**
 * Program Test NTFS Simple
 * TAHAP 1: Cuma test mount dan baca info dasar
 */

#include <stdio.h>
#include "ntfs.h"

int main(int argc, char *argv[]) {
    struct ntfs_volume volume;
    const char *disk_image = "test.img";
    
    printf("================================\n");
    printf("   NTFS SIMPLE READER - TAHAP 1\n");
    printf("================================\n\n");
    
    // Gunakan disk image dari argumen jika ada
    if (argc > 1) {
        disk_image = argv[1];
    }
    
    printf("Testing with disk image: %s\n\n", disk_image);
    
    // 1. Coba mount volume NTFS
    printf("[MAIN] Step 1: Mounting NTFS volume...\n");
    int result = ntfs_mount(&volume, disk_image);
    
    if (result != 0) {
        printf("[MAIN] Error: Mount failed with code %d\n", result);
        
        // Beri saran berdasarkan error code
        switch(result) {
            case -1:
                printf("       Cannot open file. Make sure '%s' exists.\n", disk_image);
                break;
            case -2:
                printf("       Cannot read boot sector. File mungkin corrupt.\n");
                break;
            case -3:
                printf("       Invalid boot signature. Mungkin bukan NTFS atau disk image salah.\n");
                break;
        }
        
        printf("\nSuggestion: Run 'python create_test_img.py' to create a test image.\n");
        return 1;
    }
    
    printf("[MAIN] ✓ Mount successful!\n\n");
    
    // 2. Tampilkan informasi volume
    printf("[MAIN] Step 2: Displaying volume info...\n");
    ntfs_show_info(&volume);
    
    // 3. Unmount
    printf("[MAIN] Step 3: Unmounting...\n");
    ntfs_unmount(&volume);
    
    printf("\n================================\n");
    printf("   TEST COMPLETE - TAHAP 1 OK!\n");
    printf("================================\n");
    
    printf("\nNext steps for TAHAP 2:\n");
    printf("1. Implement MFT reading\n");
    printf("2. List files in root directory\n");
    printf("3. Read small files\n");
    
    return 0;
}
