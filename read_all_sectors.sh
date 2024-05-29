#!/bin/bash

IMG_FILE="bin/hdd.img"

FILE_SIZE=$(stat --format="%s" "$IMG_FILE")

SECTOR_SIZE=512

TOTAL_SECTORS=$((FILE_SIZE / SECTOR_SIZE))

for ((i=0; i<TOTAL_SECTORS; i++))
do
    OUTPUT_FILE="sector_$i.bin"
    dd if="$IMG_FILE" bs=$SECTOR_SIZE count=1 skip=$i | hexdump -C
done

echo "All sectors have been read and saved to individual files."

