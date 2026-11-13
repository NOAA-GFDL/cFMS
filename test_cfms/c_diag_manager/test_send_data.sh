#!/bin/sh
#***********************************************************************
#*                   GNU Lesser General Public License
#*
#* This file is part of the GFDL Flexible Modeling System (FMS).
#*
#* FMS is free software: you can redistribute it and/or modify it under
#* the terms of the GNU Lesser General Public License as published by
#* the Free Software Foundation, either version 3 of the License, or (at
#* your option) any later version.
#*
#* FMS is distributed in the hope that it will be useful, but WITHOUT
#* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#* for more details.
#*
#* You should have received a copy of the GNU Lesser General Public
#* License along with FMS.  If not, see <http://www.gnu.org/licenses/>.
#***********************************************************************
# This is part of the GFDL FMS package. This is a shell script to
# execute tests in the test_fms/coupler directory.

# Set common test settings.
. ../test-lib.sh

if [ -f "input.nml" ] ; then rm -f input.nml ; fi
cat <<_EOF > input.nml
&diag_manager_nml
    use_modern_diag = .true.
/
_EOF

#from test_fms/test_diag_manager2.sh
cat <<_EOF > diag_table.yaml
title: test_diag_manager
base_date: 2 1 1 1 1 1
diag_files:
- file_name: test_send_data
  freq: 1 hours
  time_units: hours
  unlimdim: time
  varlist:
  - module: atm_mod
    var_name: var_3d
    reduction: average
    kind: r4
    output_name: var3_avg
  - module: atm_mod
    var_name: var_2d
    reduction: average
    kind: r4
    output_name: var2_avg
_EOF

test_expect_success "c_diag_manager basic diagnostics functionality with 1 pe" 'mpirun -n 1  ./test_send_data'

test_expect_success "c_diag_manager basic diagnostics functionality with 4 pes" 'mpirun -n 4  ./test_send_data'

test_done

