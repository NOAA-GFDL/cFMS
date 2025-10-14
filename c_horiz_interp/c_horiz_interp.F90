module c_horiz_interp_mod

  use FMS, only : fms_horiz_interp_init, fms_horiz_interp_del
  use FMS, only : FmsHorizInterp_type
  use FMS, only : fms_horiz_interp_new
  use FMS, only : fms_horiz_interp
  use FMS, only : fms_string_utils_c2f_string
  use FMS, only : fms_mpp_error, WARNING
  
  use c_fms_utils_mod, only : cFMS_pointer_to_array, cFMS_array_to_pointer
  use c_fms_mod, only : MESSAGE_LENGTH

  use iso_c_binding

  implicit none
  
  private

  public :: cFMS_create_xgrid_2dx2d_order1
  public :: cFMS_get_maxxgrid
  public :: cFMS_horiz_interp_init
  public :: cFMS_horiz_interp_end

  public :: cFMS_horiz_interp_new_2d_cdouble
  public :: cFMS_horiz_interp_new_2d_cfloat

  public :: cFMS_horiz_interp_base_2d_cfloat
  public :: cFMS_horiz_interp_base_2d_cdouble
  
  public :: cFMS_get_i_src, cFMS_get_i_dst
  public :: cFMS_get_j_src, cFMS_get_j_dst
  public :: cFMS_get_nlon_src, cFMS_get_nlat_src
  public :: cFMS_get_nlon_dst, cFMS_get_nlat_dst
  public :: cFMS_get_version
  public :: cFMS_get_nxgrid
  public :: cFMS_get_interp_method

  public :: cFMS_get_i_lon
  public :: cFMS_get_j_lat
  public :: cFMS_get_area_frac_dst_double
  public :: cFMS_get_area_frac_dst_float
  public :: cFMS_get_is_allocated_double
  public :: cFMS_get_is_allocated_float

  public :: cFMS_construct_interp
  
  type(FmsHorizInterp_type), allocatable, target, public :: interp(:)
  integer :: interp_count = 0
  
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

   interp_count = 0

  end subroutine cFMS_horiz_interp_init

  !cFMS_horiz_interp_end
  subroutine cFMS_horiz_interp_end() bind(C, name="cFMS_horiz_interp_end")
    implicit none
    integer :: i
    
    do i=0, size(interp)-1
       call fms_horiz_interp_del(interp(i))
    end do
    deallocate(interp)

  end subroutine cFMS_horiz_interp_end

  !cFMS_construct_interp
  !Construct the interp derived type only for r8 conservative interpolation
  !TODO:  expand to construct interp type for bilinear interpolation
  subroutine cFMS_construct_interp(interp_id, nxgrid, i_src, j_src, &
       i_dst, j_dst, area_frac_dst) bind(C, name="cFMS_construct_interp")

    implicit none
    integer, intent(in) :: interp_id
    integer, intent(in) :: nxgrid
    integer, intent(in) :: i_src(nxgrid)
    integer, intent(in) :: j_src(nxgrid)
    integer, intent(in) :: i_dst(nxgrid)
    integer, intent(in) :: j_dst(nxgrid)
    real(c_double), intent(in) :: area_frac_dst(nxgrid)

    type(FmsHorizInterp_type), pointer :: interp_p
        
    interp_p => interp(interp_id)
    interp_p%nxgrid = nxgrid
    interp_p%i_src = i_src
    interp_p%j_src = j_src
    interp_p%i_dst = i_dst
    interp_p%j_dst = j_dst
    interp_p%horizInterpReals8_type%area_frac_dst = area_frac_dst        
    
  end subroutine cFMS_construct_interp

  
#include "c_horiz_interp_int.inc"
#include "c_horiz_interp_new.fh"
#include "c_horiz_interp_base.fh"
#include "c_get_interp.fh"
  
end module c_horiz_interp_mod
