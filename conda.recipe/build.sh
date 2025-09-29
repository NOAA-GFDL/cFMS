#!/bin/bash

set -ex

FC=mpifort
CC=mpicc

CFLAGS="-fPIC -I${PREFIX}/include"
FCFLAGS="-fPIC -I${PREFIX}/include"
LDFLAGS="-L${PREFIX}/lib"

cfms_install=${PREFIX}

autoreconf -iv
./configure --enable-portable-kinds \
            --with-yaml \
            --prefix=${cfms_install}
make install
