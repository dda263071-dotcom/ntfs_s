# Simple Makefile for NTFS Reader

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = ntfs_reader
SRC_DIR = src
BUILD_DIR = build

# Semua source files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/ntfs.c $(SRC_DIR)/disk_io.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target utama
all: $(BUILD_DIR) $(TARGET)

# Buat target executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile setiap .c file ke .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Buat build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Jalankan program test
run: $(TARGET)
	@echo "Running NTFS Reader..."
	./$(TARGET) test.img || echo "\nNote: Create test image first with: python create_test_img.py"

# Buat test image
test-img:
	python create_test_img.py

# Bersihkan build
clean:
	rm -rf $(BUILD_DIR) $(TARGET) test.img

# Help
help:
	@echo "Available targets:"
	@echo "  all      : Build the program (default)"
	@echo "  run      : Build and run with test.img"
	@echo "  test-img : Create test disk image"
	@echo "  clean    : Remove build files"
	@echo "  help     : Show this help"

.PHONY: all run test-img clean help
