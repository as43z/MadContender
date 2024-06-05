#!/usr/bin/env bash
set -ex

CFLAGS=${CFLAGS:="-Wall -Wextra -pedantic -I/opt/homebrew/Cellar/gmp/6.3.0/include -Wl,/opt/homebrew/Cellar/gmp/6.3.0/lib/libgmp.a"}
COBJS=${COBJS:="main.c"}
TARGET=${TARGET:="MadContender"}
BIN_PATH=${BIN_PATH:="bin"}

if [[ ! -d $BIN_PATH ]];then
    mkdir $BIN_PATH 
fi

clang $CFLAGS $COBJS -o $BIN_PATH/$TARGET
