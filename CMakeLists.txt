#***********************************************************************
#*                             Apache License 2.0
#*
#* This file is part of the GFDL Flexible Modeling System (FMS).
#*
#* Licensed under the Apache License, Version 2.0 (the "License");
#* you may not use this file except in compliance with the License.
#* You may obtain a copy of the License at
#*
#*     http://www.apache.org/licenses/LICENSE-2.0
#*
#* FMS is distributed in the hope that it will be useful, but WITHOUT
#* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied;
#* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
#* PARTICULAR PURPOSE. See the License for the specific language
#* governing permissions and limitations under the License.
#***********************************************************************

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Build options
option(PORTABLE_KINDS "Enable compiler defition -DPORTABLE_KINDS" ON)
option(WITH_YAML "Enable compiler definition -Duse_yaml" ON)
option(CFMS_TESTS "Enable compiling of test scripts" OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

#Define the cmake project
project(cFMS
    VERSION 2026.01.0
    DESCRIPTION "C Access to GFDL FMS Library"
    HOMEPAGE_URL "https://github.com/NOAA-GFDL/cFMS"
    LANGUAGES C Fortran)

if(NOT CMAKE_C_COMPILER_ID MATCHES "^(Intel|GNU|Clang|IntelLLVM)$")
    message(
        WARNING "Compiler not officially supported: ${CMAKE_C_COMPILER_ID)}"
    )
endif()

if(NOT CMAKE_Fortran_COMPILER_ID MATCHES "^(Intel|GNU|IntelLLVM)$")
    message(
        WARNING "Compiler not officially supported: ${CMAKE_Fortran_COMPILER_ID}"
    )
endif()

# Find dependencies

FetchContent_Declare(
    FMS
    GIT_REPOSITORY https://github.com/NOAA-GFDL/FMS.git
    GIT_TAG 043ab2f6037af92997aa72a467430e95c03652ae
)
FetchContent_MakeAvailable(FMS)

list(APPEND cfms_src_files
  c_constants/c_constants.F90
  c_data_override/c_data_override.F90
  c_diag_manager/c_diag_manager.F90
  c_fms/c_fms.F90
  c_fms_utils/c_fms_utils.F90
  c_grid_utils/c_grid_utils.F90
  c_horiz_interp/c_horiz_interp.F90
)

list(APPEND cfms_header_files
  c_constants/c_constants.h
  c_data_override/c_data_override.h
  c_diag_manager/c_diag_manager.h
  c_fms/c_fms.h
  c_grid_utils/c_grid_utils.h
  c_horiz_interp/c_horiz_interp.h
)

add_library(cFMS SHARED
    ${cfms_src_files}
)

target_link_libraries(cFMS PUBLIC
  FMS::fms
  ${LIBYAML_LIBRARIES}
)

target_include_directories(cFMS PRIVATE
  c_data_override/include
  c_diag_manager/include
  c_fms/include
  c_fms_utils/include
  c_horiz_interp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/FMS/include
)

install(TARGETS cFMS
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
)
install(FILES ${cfms_header_files}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

# Tests
if(CFMS_TESTS)
    enable_testing()
    add_subdirectory(test_cfms)
endif()



