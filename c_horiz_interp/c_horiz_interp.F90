module c_horiz_interp_mod

  use FMS, only : fms_horiz_interp_init
  use FMS, only : FmsHorizInterp_type
  use FMS, only : fms_horiz_interp_new
  use FMS, only : fms_string_utils_c2f_string

  use c_fms_utils_mod, only : cFMS_pointer_to_array, cFMS_array_to_pointer
  use c_fms_mod, only : MESSAGE_LENGTH

  use iso_c_binding

  implicit none
  
  private

  public :: cFMS_create_xgrid_2dx2d_order1
  public :: cFMS_get_maxxgrid
  public :: cFMS_horiz_interp_init
  public :: cFMS_set_current_interp

  public :: cFMS_horiz_interp_2d_cdouble
  public :: cFMS_horiz_interp_2d_cfloat

  public :: cFMS_get_interp_cdouble
  public :: cFMS_get_interp_cfloat

  type(FmsHorizInterp_type), allocatable, target, public :: interp(:)
  type(fmshorizinterp_type), pointer :: current_interp
  integer, public :: current_interp_id
  
contains

  !cFMS_create_xgrid_2dx2d_order1
  function cFMS_create_xgrid_2dx2d_order1(nlon_in, nlat_in, nlon_out, nlat_out, lon_in, lat_in, lon_out, &
       lat_out, mask_in, maxxgrid, i_in, j_in, i_out, j_out, xgrid_area) bind(C, name="cFMS_create_xgrid_2dx2d_order1")

    integer, intent(in) :: nlon_in
    integer, intent(in) :: nlat_in
    integer, intent(in) :: nlon_out
    integer, intent(in) :: nlat_out
    real(c_double), intent(in) :: lon_in((nlon_in+1)*(nlat_in+1))
    real(c_double), intent(in) :: lat_in((nlon_in+1)*(nlat_in+1))
    real(c_double), intent(in) :: lon_out((nlon_out+1)*(nlat_out+1))
    real(c_double), intent(in) :: lat_out((nlon_out+1)*(nlat_out+1))
    real(c_double), intent(in) :: mask_in(nlon_in*nlat_in)
    integer, intent(in) :: maxxgrid
    real(c_double), intent(out) :: i_in(maxxgrid)
    real(c_double), intent(out) :: j_in(maxxgrid)
    real(c_double), intent(out) :: i_out(maxxgrid)
    real(c_double), intent(out) :: j_out(maxxgrid)
    real(c_double), intent(out) :: xgrid_area(maxxgrid)

    integer create_xgrid_2dx2d_order1
    integer cFMS_create_xgrid_2dx2d_order1
    
    cFMS_create_xgrid_2dx2d_order1 = create_xgrid_2dx2d_order1(nlon_in, nlat_in, nlon_out, nlat_out, lon_in, lat_in, &
         lon_out, lat_out, mask_in, i_in, j_in, i_out, j_out, xgrid_area)
    
  end function cFMS_create_xgrid_2dx2d_order1

  !cFMS_get_maxxgrid
  function cFMS_get_maxxgrid() bind(C, name="cFMS_get_maxxgrid")

    implicit none
    integer :: get_maxxgrid
    integer :: cFMS_get_maxxgrid

    cFMS_get_maxxgrid = get_maxxgrid()

  end function cFMS_get_maxxgrid

  !cFMS_horiz_interp_init
  subroutine cFMS_horiz_interp_init(ninterp) bind(C, name="cFMS_horiz_interp_init")
    implicit none
    integer, intent(in), optional :: ninterp

    call fms_horiz_interp_init

    if(present(ninterp)) then
      allocate(interp(0:ninterp-1))
    else
      allocate(interp(0:0))
    end if

  end subroutine cFMS_horiz_interp_init

  ! cFMS_get_current_interp
  function cFMS_get_current_interp()
    implicit none
    type(FmsHorizInterp_type), pointer :: cFMS_get_current_interp
    cFMS_get_current_interp => current_interp
  end function cFMS_get_current_interp

  !cFMS_set_current_interp
  subroutine cFMS_set_current_interp(interp_id) bind(C, name="cFMS_set_current_interp")

    implicit none
    integer, intent(in), optional :: interp_id
    
    if(present(interp_id)) then
       current_interp => interp(interp_id)
       current_interp_id = interp_id
    else
       current_interp => interp(0)
       current_interp_id = 0
    end if
    
  end subroutine cFMS_set_current_interp

#include "c_horiz_interp_new.fh"
#include "c_get_interp.fh"
  
end module c_horiz_interp_mod
