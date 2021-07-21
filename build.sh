#!/bin/bash

export SRC=src
export TARGET=magnolia
export TARGET_LIB=image_processing_lib
export OUT=out
export GUI=gui
export LIB=lib
export COMMON=common
CPU_CORE_NUM=`nproc --all`

mkdir -p $OUT/$SRC/$GUI
mkdir -p $OUT/$SRC/$LIB
mkdir -p $OUT/$SRC/$COMMON
make -j$CPU_CORE_NUM $1 

