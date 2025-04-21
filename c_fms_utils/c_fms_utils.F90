module c_fms_utils_mod

  use iso_c_binding
  implicit none

  private
  public :: cFMS_pointer_to_array
  public :: cFMS_array_to_pointer
  
  interface cFMS_pointer_to_array
     module procedure cFMS_pointer_to_array_2d_int
     module procedure cFMS_pointer_to_array_3d_int
     module procedure cFMS_pointer_to_array_4d_int
     module procedure cFMS_pointer_to_array_5d_int
     module procedure cFMS_pointer_to_array_2d_cfloat
     module procedure cFMS_pointer_to_array_3d_cfloat
     module procedure cFMS_pointer_to_array_4d_cfloat
     module procedure cFMS_pointer_to_array_5d_cfloat
     module procedure cFMS_pointer_to_array_2d_cdouble
     module procedure cFMS_pointer_to_array_3d_cdouble
     module procedure cFMS_pointer_to_array_4d_cdouble
     module procedure cFMS_pointer_to_array_5d_cdouble
     module procedure cFMS_pointer_to_array_2d_cbool
     module procedure cFMS_pointer_to_array_3d_cbool
     module procedure cFMS_pointer_to_array_4d_cbool
     module procedure cFMS_pointer_to_array_5d_cbool
  end interface cFMS_pointer_to_array

  interface cFMS_array_to_pointer
     module procedure cFMS_array_to_pointer_2d_int
     module procedure cFMS_array_to_pointer_3d_int
     module procedure cFMS_array_to_pointer_4d_int
     module procedure cFMS_array_to_pointer_5d_int
     module procedure cFMS_array_to_pointer_2d_cfloat
     module procedure cFMS_array_to_pointer_3d_cfloat
     module procedure cFMS_array_to_pointer_4d_cfloat
     module procedure cFMS_array_to_pointer_5d_cfloat
     module procedure cFMS_array_to_pointer_2d_cdouble
     module procedure cFMS_array_to_pointer_3d_cdouble
     module procedure cFMS_array_to_pointer_4d_cdouble
     module procedure cFMS_array_to_pointer_5d_cdouble
     module procedure cFMS_array_to_pointer_2d_logical
     module procedure cFMS_array_to_pointer_3d_logical
     module procedure cFMS_array_to_pointer_4d_logical
     module procedure cFMS_array_to_pointer_5d_logical
  end interface cFMS_array_to_pointer
  
contains

#include "pointer_to_array.fh"
#include "array_to_pointer.fh"
  
end module c_fms_utils_mod
