#!/bin/bash

DIR=$(dirname "${0}")
TARGET=$1
cat $DIR/fs_uniforms.sf $DIR/20* $DIR/10* $DIR/00* $DIR/fs_base.sf > $TARGET/stock_0.glsl.fs
cat $DIR/fs_uniforms.sf $DIR/20* $DIR/10* $DIR/01* $DIR/fs_base.sf > $TARGET/stock_1.glsl.fs
cat $DIR/fs_uniforms.sf $DIR/20* $DIR/11* $DIR/00* $DIR/fs_base.sf > $TARGET/stock_2.glsl.fs
cat $DIR/fs_uniforms.sf $DIR/20* $DIR/11* $DIR/01* $DIR/fs_base.sf > $TARGET/stock_3.glsl.fs
cat $DIR/fs_uniforms.sf $DIR/21* $DIR/10* $DIR/00* $DIR/fs_base.sf > $TARGET/stock_4.glsl.fs
cat $DIR/fs_uniforms.sf $DIR/21* $DIR/10* $DIR/01* $DIR/fs_base.sf > $TARGET/stock_5.glsl.fs
cat $DIR/fs_uniforms.sf $DIR/21* $DIR/11* $DIR/00* $DIR/fs_base.sf > $TARGET/stock_6.glsl.fs
cat $DIR/fs_uniforms.sf $DIR/21* $DIR/11* $DIR/01* $DIR/fs_base.sf > $TARGET/stock_7.glsl.fs
cp $DIR/stock.glsl.vs $TARGET/
