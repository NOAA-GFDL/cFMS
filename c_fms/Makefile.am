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

# This is an automake file for the fms directory of the FMS
# package.

# Ed Hartnett 2/22/19

# Include .h and .mod files.
AM_CPPFLAGS = -I${abs_top_srcdir}/c_fms/include
AM_FCFLAGS = $(FC_MODINC). $(FC_MODOUT)$(MODDIR) $(LIBFMS_FCFLAGS)
AM_LDFLAGS = $(LIBFMS_LIBS)

# Build these uninstalled convenience libraries.
noinst_LTLIBRARIES = lib_c_fms.la

# Each convenience library depends on its source.
lib_c_fms_la_SOURCES = c_fms.F90 

# Mod files are built and then installed as headers.
MODFILES = c_fms_mod.$(FC_MODEXT)
BUILT_SOURCES = $(MODFILES)
nodist_include_HEADERS =  $(FMS_INC_FILES) $(MODFILES)

include_HEADERS = c_fms.h

include $(top_srcdir)/mkmods.mk
