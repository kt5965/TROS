#!/bin/bash

IMG_NAME=$1

#qemu-system-i386 -m 64 -fda $IMG_NAME -M pc -hda hdd.img
qemu-system-x86_64 -m 64 -drive file=$IMG_NAME,format=raw -M pc -drive file=hdd.img,format=raw,index=1
#qemu-system-i386 -m 64 -drive file=$IMG_NAME,format=raw
#qemu-system-x86_64 -drive file=$IMG_NAME,format=raw



