#ifndef __FS_H__
#define __FS_H__

#define MAX_ENTRIES_PER_DIR 16
#define MAX_FILE_LENGTH 12

#pragma pack(push, 1)
struct BootSector {
    unsigned int fs_size;
    unsigned int block_size;
    unsigned int root_dir;
}__attribute__((packed));
#pragma pack(pop)

struct DirectoryEntry {
    char filename[MAX_FILE_LENGTH];
    unsigned int start_block;
    unsigned int size;
    unsigned char is_directory;
}__attribute__((packed));

struct Directory {
    struct DirectoryEntry entries[MAX_ENTRIES_PER_DIR];
};

void initFileSystem();
void createDirectory(char *dirname);

#endif