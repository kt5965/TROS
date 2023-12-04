#!/bin/bash

IMG_NAME=$1

qemu-system-x86_64 -drive file=$IMG_NAME,format=raw


