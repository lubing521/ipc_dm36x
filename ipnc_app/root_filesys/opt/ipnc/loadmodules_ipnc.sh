#!/bin/sh

#./av_capture_load.sh

umount /mnt/ramdisk 2>/dev/null
rmmod sbull.ko 2>/dev/null 
insmod sbull.ko 

sleep 1

mkdosfs -v /dev/sbulla 2>/dev/null
mkdir /mnt/ramdisk 2>/dev/null
mount -t vfat /dev/sbulla /mnt/ramdisk 2>/dev/null


