#!/bin/sh

SRC=src
TARGET=magnolia

make -C src $1

if [ $# -eq 1 ] 
then
	if [ $1 = clean ]
	then 
		rm $TARGET
	fi
fi

if [ -f $SRC/$TARGET ]
then
	mv $SRC/$TARGET ./
fi
