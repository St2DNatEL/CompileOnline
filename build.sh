#!/bin/bash

export SERVER_ROOT=`pwd`

if [ $# == 0 ]; then
	cd src/
	make;
	make -f makefile.obj;
	make install;

elif [ $# == 1 ]; then
	if [ $1 == 'clean' ]; then
		rm -f obj/*
		cd src;
		make clean
	fi
fi
