#***********************************************************************
#*                   GNU Lesser General Public License
#*
#* This file is part of the GFDL Flexible Modeling System (FMS).
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

# This is the main automake file for FMS.
# Ed Hartnett 2/21/2019

# This directory stores libtool macros, put there by aclocal.
ACLOCAL_AMFLAGS = -I m4

# Directory to place all .mod files.  Convienient place to look for these
# files.
#include $(top_srcdir)/mkmods.mk

# Does the user want to build documentation?
if BUILD_DOCS
DOCS = docs
endif

# Make targets will be run in each subdirectory. Order is significant.
if BUILD_FMS
SUBDIRS = FMS
else
SUBDIRS =
endif
SUBDIRS += c_fms_utils \
          c_constants \
          c_fms \
          c_data_override \
          c_diag_manager \
          c_grid_utils \
          c_horiz_interp \
          libcFMS \
          test_cfms

## Build libFMS module
AM_CPPFLAGS = -I${abs_top_srcdir}/include -I$(abs_top_builddir)/FMS/include
AM_FCFLAGS = $(FC_MODINC). $(FC_MODOUT)$(MODDIR)
AM_LDFLAGS = -L$(abs_top_builddir)/libFMS -lFMS

include $(top_srcdir)/mkmods.mk

#clean-local:
#	-rm -f FMSConfig.cmake FMSConfigVersion.cmake
EXTRA_DIST = README.md #_package_init_cmake


install-data-hook:
	@echo ''
	@echo '+-------------------------------------------------------------+'
	@echo '| Congratulations! You have successfully installed the FMS    |'
	@echo '| Fortran library.                                            |'
	@echo '|                                                             |'
	@echo '| After the installed include and link paths have been        |'
	@echo '| specified, code using FMS should be compiled using the      |'
	@echo '| "-lFMS" flag.                                               |'
	@echo '|                                                             |'
	@echo '| FMS is developed and maintained at the GFDL publicly on     |'
	@echo '| Github. To report an issue or view available documentation, |'
	@echo '| please see our page: https://www.github.com/NOAA-GFDL/FMS   |'
	@echo '+-------------------------------------------------------------+'
	@echo ''

