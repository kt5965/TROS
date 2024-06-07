#include "fs.h"
#include "function.h"
#include "data.h"



void initFileSystem() {
    
    struct BootSector bs;
    struct Directory root_dir;
    
    bs.fs_size = 2048;
    bs.block_size = 512;
    bs.root_dir = 2;
    current_directory = bs.root_dir;
    HDDwrite(1, 1, (char*)&bs);

    printf("FS size: %d, Block size: %d, Root dir: %d", bs.fs_size, bs.block_size, bs.root_dir);

    kmemset((char*)&root_dir, 0, sizeof(struct Directory));
    HDDwrite(1, 2, (char*)&root_dir);
}

void createDirectory(char* dirname) {
    struct DirectoryEntry new_entry;
    struct Directory current_dir;
    struct BootSector bs2;
    unsigned char test[512];

    HDDread(1, current_directory, test);
    printf("%d", current_directory);
    kmemcpy((char*)&current_dir, test, sizeof(struct BootSector));
    kmemset((char*)&current_dir, 0, sizeof(struct Directory));
    kmemset((char*)&new_entry, 0, sizeof(struct DirectoryEntry));
    kstrcpy(new_entry.filename, dirname);

    new_entry.start_block = current_directory + 1;
    printf("%d", new_entry.start_block);
    new_entry.is_directory = 1;
    for (int i=0; i < MAX_ENTRIES_PER_DIR; i++) {
        if (current_dir.entries[i].filename[0] == '\0') {
            current_dir.entries[i] = new_entry;
            printf("%s", current_dir.entries[i].filename);
            printf("%d", current_dir.entries[i].start_block);
            break;
        }
    }

    HDDwrite(1, current_directory, (char*)&current_dir);
    printf("make directory complete %s",new_entry.filename);
}
