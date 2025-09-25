#!/bin/bash

set -ex

FC=mpifort
CC=mpicc

CFLAGS="-fPIC -I${PREFIX}/include"
FCFLAGS="-fPIC -I${PREFIX}/include"
LDFLAGS="-L${PREFIX}/lib"

curr_dir=$PWD

fms_install="${PREFIX}"
cfms_install="${PREFIX}"

cfms_dir=$curr_dir/cFMS
fms_dir=$curr_dir/FMS

# cd $fms_dir
# autoreconf -iv
# ./configure --enable-portable-kinds \
#             --with-yaml \
#             --prefix=$fms_install
# make install

cd $curr_dir
autoreconf -iv
./configure --with-fms=$fms_install \
            --prefix=$cfms_install
make install
