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
AM_CPPFLAGS = -I. -I$(MODDIR) -I${top_builddir}/c_diag_manager \
              -I${top_builddir}/c_fms -I${top_builddir}/test_cfms/c_fms
AM_FCFLAGS = $(LIBFMS_FCFLAGS)
AM_LDFLAGS = $(LIBFMS_LIBS)

# Link to the FMS library.
LDADD = ${top_builddir}/libcFMS/libcFMS.la

check_PROGRAMS = test_send_data

TESTS = test_send_data.sh

test_send_data_SOURCES = ../c_fms/c_mpp_domains_helper.c test_send_data.c

TEST_EXTENSIONS = .sh
SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
                  $(abs_top_srcdir)/test_cfms/tap-driver.sh

# Include these files with the distribution.
EXTRA_DIST = test_send_data.sh

# Clean up
CLEANFILES = input.nml  *.out *.dpi *.spi *.dyn *.spl *_table* input* *trs *.nc* *.yaml*


