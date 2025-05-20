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
AM_CPPFLAGS = -I. -I$(MODDIR) -I${abs_top_srcdir}/c_fms
AM_FCFLAGS = $(LIBFMS_FCFLAGS)
AM_LDFLAGS = $(LIBFMS_LIBS)

# Link to the FMS library.
LDADD = ${abs_top_builddir}/libcFMS/libcFMS.la

check_PROGRAMS = test_utils

TESTS = test_utils.sh

test_utils_SOURCES = test_utils.F90 test_utils_c.c 

test_utils_c.o : test_utils_c.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $(AM_CPPFLAGS) test_utils_c.c -o test_utils_c.o

test_utils.o : test_utils.F90
	$(FC) -c $(FCFLAGS) $(LDFLAGS) $(AM_CPPFLAGS) test_utils.F90 -o test_utils.o

TEST_EXTENSIONS = .sh
SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
                  $(abs_top_srcdir)/test_cfms/tap-driver.sh

# Include these files with the distribution.
EXTRA_DIST = test_utils.sh

# Clean up
CLEANFILES = input.nml *.nc* *.out *.dpi *.spi *.dyn *.spl *_table* input* *trs

