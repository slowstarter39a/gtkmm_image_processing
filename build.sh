#!/bin/sh

export SRC=src
export TARGET=magnolia
export TARGET_LIB=image_process_lib
export OUT=out
export GUI=gui
export LIB=lib
CPU_CORE_NUM=`nproc --all`

mkdir -p $OUT/$SRC/$GUI
mkdir -p $OUT/$SRC/$LIB
make -j$CPU_CORE_NUM $1 

