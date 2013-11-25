#!/bin/sh

module="csl"

./csl_unload.sh 2>/dev/null

# invoke insmod with all arguments we got
insmod ./$module.ko || exit 1

# retrieve major number
major=$(awk '$2=="csl" {print $1}' /proc/devices)

rm -f /dev/${module}[0-9]
mknod /dev/${module}0 c $major 0
mknod /dev/${module}1 c $major 1
mknod /dev/${module}2 c $major 2
mknod /dev/${module}3 c $major 3
mknod /dev/${module}4 c $major 4
mknod /dev/${module}5 c $major 5
mknod /dev/${module}6 c $major 6
mknod /dev/${module}7 c $major 7
mknod /dev/${module}8 c $major 8
mknod /dev/${module}9 c $major 9






