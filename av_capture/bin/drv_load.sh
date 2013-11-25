
./drv_unload.sh 2>/dev/null

insmod ./drv.ko

if [ ! -f /dev/dev_i2c ]
then
    major=$(awk '$2=="dev_i2c" {print $1}' /proc/devices)
    mknod /dev/dev_i2c c ${major} 0
fi

if [ ! -f /dev/dev_dma ]
then
    major=$(awk '$2=="dev_dma" {print $1}' /proc/devices)
    mknod /dev/dev_dma c ${major} 0
fi

