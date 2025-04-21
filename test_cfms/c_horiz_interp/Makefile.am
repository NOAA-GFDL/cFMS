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
#

# Find the needed mod and .inc files.
AM_CPPFLAGS = -I. -I$(MODDIR) -I${top_builddir}/c_constants -I${top_builddir}/c_grid_utils \
              -I${top_builddir}/c_horiz_interp -I${top_builddir}/c_fms \
              -I${top_builddir}/test_cfms/c_fms

# Link to the FMS library.
LDADD = ${top_builddir}/libcFMS/libcFMS.la

check_PROGRAMS = test_create_xgrid \
  test_horiz_interp_new

TESTS = test_create_xgrid.sh \
  test_horiz_interp_new.sh

test_create_xgrid_SOURCES = test_create_xgrid.c
test_horiz_interp_new_SOURCES = ../c_fms/c_mpp_domains_helper.c test_horiz_interp_new.c

TEST_EXTENSIONS = .sh
SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
                  $(abs_top_srcdir)/test_cfms/tap-driver.sh

# Include these files with the distribution.
EXTRA_DIST = test_create_xgrid.sh \
  test_horiz_interp_new.sh

# Clean up
CLEANFILES = input.nml *.nc* *.out *.dpi *.spi *.dyn *.spl *_table* input* *trs

