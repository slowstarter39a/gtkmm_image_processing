#!/bin/bash

export SRC=src
export TARGET=magnolia
export TARGET_LIB=image_processing_lib
export OUT=out
export GUI=gui
export LIB=lib
export COMMON=common
export PRJ_ROOT=`pwd`
export CPU_CORE_NUM=`nproc --all`

mkdir -p $OUT/$SRC/$GUI
mkdir -p $OUT/$SRC/$LIB
mkdir -p $OUT/$SRC/$COMMON

make $1

if [ $? -eq 0 ] ; then
	echo
	echo -e "\e[1;32mCongratulations! The build was successful!\e[0m"
	echo -e "\e[1;32mI hope the functions you implemented work well!\e[0m\n"
else
	echo
	echo -e "\e[41;1;37mIt's really disappointing to you that the build failed!\e[0m"
	echo -e "\e[41;1;37mPlease fix it as soon as possible before you leave!\e[0m\n"
fi
