#!/bin/sh

export SRC=src
export TARGET=magnolia
export OUT=out
CPU_CORE_NUM=`nproc --all`

mkdir -p $OUT/$SRC
make -j$CPU_CORE_NUM $1 

