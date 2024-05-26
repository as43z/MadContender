#!/usr/bin/env bash
set -ex

CFLAGS=${CFLAGS:="-Wall -Wextra -pedantic -Ithirdparties/gmp-6.3.0 -Wl,thirdparties/gmp-6.3.0/.libs/libgmp.a"}
COBJS=${COBJS:="main.c"}
TARGET=${TARGET:="MadContender"}
BIN_PATH=${BIN_PATH:="bin"}

if [[ ! -d $BIN_PATH ]];then
    mkdir $BIN_PATH 
fi

clang $CFLAGS $COBJS -o $BIN_PATH/$TARGET
