#!/bin/bash

# debug output
set -x
# exit on command error
set -e

curr_dir=$PWD
install_fms=$curr_dir/FMS/gnuFMS

cd FMS
autoreconf -iv
export FCFLAGS="$FCFLAGS -fPIC"
export CFLAGS="$CFLAGS -fPIC"
./configure --enable-portable-kinds --with-yaml --prefix=$install_fms
make install
cd $curr_dir

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$install_fms/lib"

autoreconf -iv
./configure --with-fms=$install_fms --prefix=$curr_dir/cgnuFMS
make check
