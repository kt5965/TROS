#pragma once

struct BootSector {
    unsigned int fs_size;
    unsigned int block_size;
    unsigned int root_dir;
} bs;

struct DirectoryEntry {
    char filename[12];
    unsigned int start_block;
}

void write_filesystem();

