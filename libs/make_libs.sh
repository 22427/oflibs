#!/bin/bash

DIR=$(dirname "${0}")
LIBS=$1
TARGET=$2

for f in $LIBS/*.h;
do
		filename=$(basename "$f")
		extension="${filename##*.}"
		filename="${filename%.*}"
		../bin/ofl_creator $f > $TARGET/ofl_${filename}.h
done

