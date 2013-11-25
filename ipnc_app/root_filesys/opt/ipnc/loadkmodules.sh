# insmod - insert kernel modules - ANR
#modules (.ko) - /home/anand/home1/filesys2/root/modules/
#
#insmod list as follows,
#
#init.d modules
#/opt/ipnc/modules/init.d_ins/checkroot.sh
/opt/ipnc/modules/init.d_ins/mountall.sh
/opt/ipnc/modules/init.d_ins/ifupdown start
/opt/ipnc/modules/init.d_ins/hotplug start
/opt/ipnc/modules/init.d_ins/portmap start
/opt/ipnc/modules/init.d_ins/inetd start
#
#
#
#Kernel modules
#other
insmod /opt/ipnc/modules/crc-ccitt.ko
insmod /opt/ipnc/modules/libcrc32c.ko
insmod /opt/ipnc/modules/deadline-iosched.ko
insmod /opt/ipnc/modules/cfq-iosched.ko
#insmod /opt/ipnc/modules/i2c-emac.ko cmdline="`cat /proc/cmdline`"
insmod /opt/ipnc/modules/sbull.ko
#cripto
insmod /opt/ipnc/modules/crc32c.ko
insmod /opt/ipnc/modules/deflate.ko
insmod /opt/ipnc/modules/des.ko
insmod /opt/ipnc/modules/md5.ko
insmod /opt/ipnc/modules/sha1.ko
#fs
insmod /opt/ipnc/modules/nls_ascii.ko
insmod /opt/ipnc/modules/nls_utf8.ko
insmod /opt/ipnc/modules/exportfs.ko
insmod /opt/ipnc/modules/nfsd.ko
#driver
#
#net
insmod /opt/ipnc/modules/loop.ko
insmod /opt/ipnc/modules/tun.ko
insmod /opt/ipnc/modules/slhc.ko
insmod /opt/ipnc/modules/netconsole.ko
insmod /opt/ipnc/modules/ppp_generic.ko
insmod /opt/ipnc/modules/ppp_synctty.ko
insmod /opt/ipnc/modules/ppp_async.ko
insmod /opt/ipnc/modules/ppp_deflate.ko
#insmod /opt/ipnc/modules/davinci_emac.ko
#mmc
insmod /opt/ipnc/modules/mmc_core.ko
insmod /opt/ipnc/modules/mmc_block.ko
insmod /opt/ipnc/modules/davinci-mmc.ko
#usb
insmod /opt/ipnc/modules/usbcore.ko
insmod /opt/ipnc/modules/musb_hdrc.ko
insmod /opt/ipnc/modules/usbmon.ko
#insmod /opt/ipnc/modules/g_file_storage.ko
#scsi
insmod /opt/ipnc/modules/scsi_transport_iscsi.ko
insmod /opt/ipnc/modules/libiscsi.ko
insmod /opt/ipnc/modules/iscsi_tcp.ko
#
#
#
#/opt/ipnc/dhcpcd/dhcpcd
#
#
#
