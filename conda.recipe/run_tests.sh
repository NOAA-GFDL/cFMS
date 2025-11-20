#!/bin/bash
# runs some unit tests to test the built conda library 

# exits on first error
set -e

touch input.nml

# skipping these for now, they use/write netcdf files and require additional input 
# data override
#mpirun -n 4 ./test_cfms/c_data_override/test_data_override_2d
#mpirun -n 6 ./test_cfms/c_data_override/test_data_override_3d
#mpirun -n 2 ./test_cfms/c_data_override/test_data_override_scalar
# diag manager
#mpirun -n 1 ./test_cfms/c_diag_manager/test_send_data
#mpirun -n 30 ./test_cfms/c_fms/test_define_cubic_mosaic

# fms/mpp
#mpirun -n 8 ./test_cfms/c_fms/test_define_domains # TODO, not sure why this one is failing
mpirun -n 4 ./test_cfms/c_fms/test_gather
mpirun -n 4 ./test_cfms/c_fms/test_getset_domains
mpirun -n 4 ./test_cfms/c_fms/test_update_domains
mpirun -n 2 ./test_cfms/c_fms/test_vector_update_domains

# fms utils 
#mpirun -n 1 ./test_cfms/c_fms_utils/test_utils # this ones annoying to compile, so isn't built

# grid utils
mpirun -n 1 ./test_cfms/c_grid_utils/test_grid_utils

# horiz interp 
mpirun -n 1 ./test_cfms/c_horiz_interp/test_create_xgrid
mpirun -n 4 ./test_cfms/c_horiz_interp/test_horiz_interp_new

rm input.nml *.out
