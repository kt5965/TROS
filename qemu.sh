#!/bin/bash

IMG_NAME=$1

pushd . && cd bin && qemu-img create -f raw hdd.img 1M && popd




qemu-system-x86_64 -m 64 \
    -drive file=$IMG_NAME,format=raw \
    -drive file=bin/hdd.img,format=raw \
    -vnc :1