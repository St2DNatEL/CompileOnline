#!/bin/bash
cd /var/ftp/pub/MyProject/CompileOnline/bin/

BIN=compileonline.bin

cnt=`ps -ef | grep $BIN | grep -v grep | wc -l`
echo $cnt
if [ $cnt -ge 1 ]; then
	echo "stop $BIN ..."
	killall $BIN
fi
echo "start $BIN..."
./$BIN >> log.log 2>&1 &
echo "start OK"
