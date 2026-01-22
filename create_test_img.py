#!/usr/bin/env python3
"""
Create a simple NTFS test disk image for testing
Hanya 10MB dengan struktur NTFS valid
"""

import os
import struct
import subprocess
import sys

def create_test_image(filename="test.img", size_mb=10):
    """Create a minimal NTFS test image"""
    
    print(f"Creating NTFS test image: {filename} ({size_mb}MB)")
    
    # 1. Buat file kosong dengan size yang ditentukan
    with open(filename, 'wb') as f:
        f.write(b'\x00' * (size_mb * 1024 * 1024))
    
    # 2. Format dengan NTFS menggunakan mkfs.ntfs atau mkntfs
    # Cek dulu apakah command tersedia
    mkfs_cmd = None
    for cmd in ['mkfs.ntfs', 'mkntfs', 'mkfs -t ntfs']:
        try:
            subprocess.run([cmd.split()[0], '--version'], 
                          stdout=subprocess.DEVNULL, 
                          stderr=subprocess.DEVNULL)
            mkfs_cmd = cmd
            break
        except FileNotFoundError:
            continue
    
    if not mkfs_cmd:
        print("Error: Could not find mkfs.ntfs or mkntfs command")
        print("On Ubuntu/Debian, install with: sudo apt-get install ntfs-3g")
        print("On macOS with Homebrew: brew install ntfs-3g")
        
        # Buat disk image manual sederhana (tanpa filesystem lengkap)
        print("\nCreating minimal disk image with NTFS signature only...")
        create_minimal_image(filename)
        return
    
    # 3. Format dengan filesystem NTFS
    print(f"Formatting with {mkfs_cmd}...")
    
    if 'mkfs -t ntfs' in mkfs_cmd:
        cmd_args = ['mkfs', '-t', 'ntfs', '-F', filename]
    else:
        cmd_args = [mkfs_cmd.split()[0], '-F', '-L', 'TEST_NTFS', filename]
    
    try:
        subprocess.run(cmd_args, check=True)
        
        # 4. Buat beberapa file test
        print("Creating test files...")
        
        # Mount (di Linux/Mac) untuk tambah file
        # Note: Ini butuh sudo atau permission khusus
        # Untuk sekarang, cukup filesystem kosong saja
        print("Test image created successfully!")
        print(f"\nYou can manually add test files with:")
        print(f"  sudo mount -t ntfs-3g {filename} /mnt")
        print(f"  sudo cp some_file.txt /mnt/")
        print(f"  sudo umount /mnt")
        
    except subprocess.CalledProcessError as e:
        print(f"Error formatting: {e}")
        create_minimal_image(filename)

def create_minimal_image(filename):
    """Create minimal disk image with NTFS boot sector only"""
    
    print("Creating minimal NTFS-like image...")
    
    with open(filename, 'wb') as f:
        # Boot sector NTFS minimal
        boot_sector = bytearray(512)
        
        # Jump instruction
        boot_sector[0] = 0xEB
        boot_sector[1] = 0x52
        boot_sector[2] = 0x90
        
        # OEM ID
        boot_sector[3:11] = b'NTFS    '
        
        # Bytes per sector (512 = 0x0200)
        boot_sector[11] = 0x00
        boot_sector[12] = 0x02
        
        # Sectors per cluster (8)
        boot_sector[13] = 0x08
        
        # Reserved sectors (0)
        boot_sector[14:16] = b'\x00\x00'
        
        # Media descriptor (F8 = hard disk)
        boot_sector[21] = 0xF8
        
        # Total sectors (dummy value)
        struct.pack_into('<Q', boot_sector, 40, 20480)  # 10MB dengan 512 byte/sector
        
        # MFT start cluster (cluster 4)
        struct.pack_into('<Q', boot_sector, 48, 4)
        
        # Signature AA55 di akhir
        boot_sector[510] = 0x55
        boot_sector[511] = 0xAA
        
        f.write(boot_sector)
        
        # Isi sisanya dengan nol
        f.write(b'\x00' * (10 * 1024 * 1024 - 512))
    
    print(f"Minimal image created: {filename}")
    print("Note: This is not a fully valid NTFS filesystem,")
    print("      but has correct boot sector signature for testing.")

def main():
    """Main function"""
    print("=" * 50)
    print("NTFS  Disk Image")
    print("=" * 50)
    
    # Tentukan nama file
    filename = "test.img"
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    
    # Buat image
    create_test_image(filename)
    
    print("\nTo compile and test the NTFS reader:")
    print("  make           # Build the program")
    print("  make test-img  # Create test image (if not already)")
    print("  make run       # Build and run")
    print("\nOr manually:")
    print(f"  gcc -Wall -std=c99 src/*.c -o ntfs_reader")
    print(f"  ./ntfs_reader {filename}")

if __name__ == "__main__":
    main()
