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

check_PROGRAMS = test_define_domains \
  test_gather_cint \
  test_gather_cfloat \
  test_gather_cdouble \
  test_getset_domains \
  test_update_domains \
  test_vector_update_domains \
  test_define_cubic_mosaic \
  test_cfms

TESTS = test_define_domains.sh \
  test_gather.sh \
  test_getset_domains.sh \
  test_update_domains.sh \
  test_vector_update_domains.sh \
  test_define_cubic_mosaic.sh \
  test_cfms.sh

test_gather_cint_SOURCES = c_mpp_domains_helper.c test_gather.c
test_gather_cfloat_SOURCES = c_mpp_domains_helper.c test_gather.c
test_gather_cdouble_SOURCES = c_mpp_domains_helper.c test_gather.c
test_define_domains_SOURCES = c_mpp_domains_helper.c test_define_domains.c
test_getset_domains_SOURCES = c_mpp_domains_helper.c test_getset_domains.c
test_update_domains_SOURCES = c_mpp_domains_helper.c test_update_domains.c
test_vector_update_domains_SOURCES = c_mpp_domains_helper.c test_vector_update_domains.c
test_define_cubic_mosaic_SOURCES = c_mpp_domains_helper.c test_define_cubic_mosaic.c
test_fms_SOURCES = test_fms.c

test_gather_cint_CPPFLAGS = -DCFMS_TEST_KIND_=int \
                            -DCFMS_GATHER_1D_=cFMS_gather_1d_cint \
                            -DCFMS_GATHER_PELIST_2D_=cFMS_gather_pelist_2d_cint \
                            -DCFMS_GATHERV_1D_=cFMS_gatherv_1d_cint \
                            $(AM_CPPFLAGS)

test_gather_cfloat_CPPFLAGS = -DCFMS_TEST_KIND_=float \
                              -DCFMS_GATHER_1D_=cFMS_gather_1d_cfloat \
                              -DCFMS_GATHER_PELIST_2D_=cFMS_gather_pelist_2d_cfloat \
                              -DCFMS_GATHERV_1D_=cFMS_gatherv_1d_cfloat \
                              $(AM_CPPFLAGS)

test_gather_cdouble_CPPFLAGS = -DCFMS_TEST_KIND_=double \
                               -DCFMS_GATHER_1D_=cFMS_gather_1d_cdouble \
                               -DCFMS_GATHER_PELIST_2D_=cFMS_gather_pelist_2d_cdouble \
                               -DCFMS_GATHERV_1D_=cFMS_gatherv_1d_cdouble \
                               $(AM_CPPFLAGS)

TEST_EXTENSIONS = .sh
SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
                  $(abs_top_srcdir)/test_cfms/tap-driver.sh

# Include these files with the distribution.
EXTRA_DIST = test_define_domains.sh \
  test_gather.sh \
  test_getset_domains.sh \
  test_update_domains.sh \
  test_vector_update_domains.sh \
  test_define_cubic_mosaic.sh \
  test_cfms.sh

# Clean up
CLEANFILES = input.nml *.nc* *.out *.dpi *.spi *.dyn *.spl *_table* input* *trs

