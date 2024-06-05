#include "fs.h"
#include "function.h"
#include "data.h"

unsigned int current_directory = 2;

void initFileSystem() {
    struct BootSector bs;
    struct BootSector bs2;
    unsigned char test[512];
    struct Directory root_dir;
    printf("size = %d", sizeof(bs));
    bs.fs_size = 2048;
    bs.block_size = 512;
    bs.root_dir = 2;

    HDDwrite(1, 1, (char*)&bs);
    HDDread(1, 1, test);
    kmemcpy((char*)&bs2, test, sizeof(struct BootSector));
    kmemset((char*)&root_dir, 0, sizeof(struct Directory));
    printf("FS size: %d, Block size: %d, Root dir: %d", bs2.fs_size, bs2.block_size, bs2.root_dir);
    HDDwrite(1, 2, (char*)&root_dir);
}

void createDirectory(char* dirname) {
    struct DirectoryEntry new_entry;
    struct Directory current_dir;
    struct BootSector bs2;
    unsigned char test[512];
    HDDread(1, current_directory, test);
    kmemcpy((char*)&current_dir, test, sizeof(struct BootSector));
    kmemset((char*)&new_entry, 0, sizeof(struct DirectoryEntry));
    kstrcpy(new_entry.filename, dirname);
    new_entry.start_block = current_directory + 1;
    new_entry.is_directory = 1;
    kmemset((char*)&current_dir, 0, sizeof(struct Directory));

    for (int i=0; i < MAX_ENTRIES_PER_DIR; i++) {
        if (current_dir.entries[i].filename == 0) {
            current_dir.entries[i] = new_entry;
            break;
        }
    }

    HDDwrite(1, current_directory, (char*)&current_dir);
    printf("make directory complete %s",new_entry.filename);
}
    // directory entry setting
