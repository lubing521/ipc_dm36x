#!/bin/sh

killall -KILL av_server.out
killall wis-streamer

./csl_unload.sh
./drv_unload.sh

##to check with kedar cd ../dvsdk/dm365

rmmod cmemk.ko 
rmmod irqk.ko
rmmod edmak.ko 

rm -f /dev/dm365mmap
rmmod dm365mmap.ko

# cd -


