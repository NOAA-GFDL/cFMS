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
AM_CPPFLAGS = -I. -I$(MODDIR) -I${top_builddir}/c_fms

# Link to the FMS library.
LDADD = ${top_builddir}/libcFMS/libcFMS.la

check_PROGRAMS = test_define_domains \
	test_getset_domains \
	test_update_domains \
  test_vector_update_domains

TESTS = test_define_domains.sh \
	test_getset_domains.sh \
  test_update_domains.sh test_vector_update_domains.sh

test_define_domains_SOURCES = c_mpp_domains_helper.c test_define_domains.c 
test_getset_domains_SOURCES = c_mpp_domains_helper.c test_getset_domains.c
test_update_domains_SOURCES = c_mpp_domains_helper.c test_update_domains.c
test_vector_update_domains_SOURCES = c_mpp_domains_helper.c test_vector_update_domains.c 

TEST_EXTENSIONS = .sh
SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
                  $(abs_top_srcdir)/test_cfms/tap-driver.sh

# Include these files with the distribution.
EXTRA_DIST = test_define_domains.sh \
  test_getset_domains.sh \
  test_update_domains.sh \
  test_vector_update_domains.sh

# Clean up
CLEANFILES = input.nml *.nc* *.out *.dpi *.spi *.dyn *.spl *_table* input* *trs

