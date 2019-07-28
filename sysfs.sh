#!/bin/bash

#Populate block devices

for i in /sys/block/*/dev /sys/block/*/*/dev
do
	if [ -f $i ]
	then
		MAJOR = $(sed 's/:.*//' < $i)
		MINOR = $(sed 's/.*://' < $i)
		DEVNAME = $(echo $i | sed -e '$@/dev@@' -e '$@.*/@@')
		echo /dev/$DEVNAME b $MAJOR $MINOR
		#mknod /dev/$DEVNAME b $MAJOR $MINOR
	fi
done

for i in /sys/bus/*/devices/*/dev /sys/class/*/*/dev
do
	if [ -f $i ]
	then
		MAJOR = $(sed 's/:.*//' < $i)
		MINOR = $(sed 's/.*://' < $i)
		DEVNAME = $(echo $i | sed -e '$@/dev@@' -e '$@.*/@@')
		echo /dev/$DEVNAME b $MAJOR $MINOR
		#mknod /dev/$DEVNAME b $MAJOR $MINOR
	fi
done
