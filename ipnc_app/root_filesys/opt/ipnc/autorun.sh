#echo "2" > /proc/cpu/alignment
sleep 1
./boa -c /etc &
/usr/sbin/avahi-autoipd --no-drop-root --no-chroot -D eth0
/usr/sbin/avahi-daemon --no-drop-root --no-chroot -D
/usr/sbin/avahi-dnsconfd -D
