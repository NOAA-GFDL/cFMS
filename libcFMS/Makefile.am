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

# This is an automake file for the libFMS directory of the FMS
# package. This is the final packaging of the library.

# Ed Hartnett 2/22/19

# This builds the FMS library file.
lib_LTLIBRARIES = libcFMS.la

# These linker flags specify libtool version info.
# See http://www.gnu.org/software/libtool/manual/libtool.html#Libtool-versioning
# for information regarding incrementing `-version-info`.
libcFMS_la_LDFLAGS = -version-info 21:0:0

# Add the convenience libraries to the FMS library.
libcFMS_la_LIBADD = $(top_builddir)/c_fms/lib_c_fms.la
libcFMS_la_LIBADD += $(top_builddir)/c_constants/lib_c_constants.la
libcFMS_la_LIBADD += $(top_builddir)/c_data_override/lib_c_data_override.la
libcFMS_la_LIBADD += $(top_builddir)/c_diag_manager/lib_c_diag_manager.la
libcFMS_la_LIBADD += $(top_builddir)/c_horiz_interp/lib_c_horiz_interp.la
libcFMS_la_LIBADD += $(top_builddir)/c_grid_utils/lib_c_grid_utils.la
libcFMS_la_LIBADD += $(top_builddir)/c_fms_utils/lib_c_fms_utils.la

if BUILD_FMS
libcFMS_la_LIBADD += $(abs_top_builddir)/FMS/libFMS/libFMS.la
libcFMS_la_LDFLAGS += -L$(abs_top_builddir)/FMS/libFMS/
endif

libcFMS_la_SOURCES =

nodist_EXTRA_libcFMS_la_SOURCES = dummy.f90
