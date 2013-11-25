#!/bin/sh

module="csl"

# invoke rmmod with all arguments we got
rmmod $module || exit 1

# Remove stale nodes
rm -f /dev/${module}[0-9] 



