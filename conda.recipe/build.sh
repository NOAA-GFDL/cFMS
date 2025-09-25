#!/bin/bash

set -ex

FC=mpif90
CC=mpicc

CFLAGS="${CFLAGS} -fPIC -I${PREFIX}/include"
FCFLAGS="${FCFLAGS} -fPIC -I${PREFIX}/include"
LDFLAGS="${LDFLAGS} -L${PREFIX}/lib"


curr_dir=$PWD

fms_install=${PREFIX}/lib/FMS
cfms_install=${PREFIX}/lib/cFMS

cfms_dir=$curr_dir/cFMS
fms_dir=$curr_dir/FMS

cd $fms_dir
autoreconf -iv
./configure --enable-portable-kinds \
            --with-yaml \
            --prefix=$fms_install \
            FC=$FC \
            CC=$CC \
            FCFLAGS="$FCFLAGS" \
            CFLAGS="$CFLAGS" \
            LDFLAGS="$LDFLAGS"
make install

cd $curr_dir

cd $cfms_dir
autoreconf -iv
./configure --with-fms=$fms_install \
            --prefix=$cfms_install \
            FC=$FC \
            CC=$CC \
            FCFLAGS="$FCFLAGS" \
            CFLAGS="$CFLAGS" \
            LDFLAGS="$LDFLAGS"
make install
