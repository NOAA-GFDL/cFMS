module c_horiz_interp_mod

  use FMS, only : fms_horiz_interp_init
  use FMS, only : FmsHorizInterp_type
  use FMS, only : fms_horiz_interp_new
  use FMS, only : fms_string_utils_c2f_string
  use FMS, only : fms_mpp_error, FATAL
  use horiz_interp_type_mod, only : CONSERVE, BILINEAR

  use c_fms_utils_mod, only : cFMS_pointer_to_array, cFMS_array_to_pointer
  use c_fms_mod, only : MESSAGE_LENGTH

  use iso_c_binding

  implicit none
  
  private

  public :: cFMS_create_xgrid_2dx2d_order1
  public :: cFMS_get_maxxgrid
  public :: cFMS_horiz_interp_init
  public :: cFMS_horiz_interp_dealloc
  public :: cFMS_set_current_interp

  public :: cFMS_horiz_interp_2d_cdouble
  public :: cFMS_horiz_interp_2d_cfloat

  public :: cFMS_get_interp_cdouble
  public :: cFMS_get_interp_cfloat

  public :: cFMS_get_i_src, cFMS_get_i_dst
  public :: cFMS_get_j_src, cFMS_get_j_dst
  public :: cFMS_get_nlon_src, cFMS_get_nlat_src
  public :: cFMS_get_nlon_dst, cFMS_get_nlat_dst
  public :: cFMS_get_version
  public :: cFMS_get_nxgrid
  public :: cFMS_get_current_interp
  public :: cFMS_get_interp_method

  public :: cFMS_get_i_lon_double
  public :: cFMS_get_j_lat_double
  public :: cFMS_get_area_frac_dst_double
  public :: cFMS_get_area_frac_dst_float
  public :: cFMS_get_is_allocated_double
  public :: cFMS_get_i_lon_float
  public :: cFMS_get_j_lat_float
  public :: cFMS_get_is_allocated_float

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

  !cFMS_horiz_interp_dealloc
  subroutine cFMS_horiz_interp_dealloc() bind(C, name="cFMS_horiz_interp_dealloc")
    implicit none

    if(allocated(interp)) deallocate(interp)
    nullify(current_interp)

  end subroutine cFMS_horiz_interp_dealloc

  subroutine cFMS_get_i_src(interp_id, i_src) bind(C, name="cFMS_get_i_src")
      integer, intent(in) :: interp_id
      type(c_ptr), intent(in), value :: i_src
      integer, pointer :: i_src_cf(:)
      call cFMS_set_current_interp(interp_id)
      if(C_ASSOCIATED(i_src)) then
          call C_F_POINTER(i_src, i_src_cf, shape=(/current_interp%nxgrid/))
          i_src_cf = current_interp%i_src
      end if
      NULLIFY(i_src_cf)
  end subroutine cFMS_get_i_src

  subroutine cFMS_get_j_src(interp_id, j_src) bind(C, name="cFMS_get_j_src")
      integer, intent(in) :: interp_id
      type(c_ptr), intent(in), value :: j_src
      integer, pointer :: j_src_cf(:)
      call cFMS_set_current_interp(interp_id)
      if(C_ASSOCIATED(j_src)) then
          call C_F_POINTER(j_src, j_src_cf, shape=(/current_interp%nxgrid/))
          j_src_cf = current_interp%j_src
      end if
      NULLIFY(j_src_cf)
  end subroutine cFMS_get_j_src

  subroutine cFMS_get_i_dst(interp_id, i_dst) bind(C, name="cFMS_get_i_dst")
      integer, intent(in) :: interp_id
      type(c_ptr), intent(in), value :: i_dst
      integer, pointer :: i_dst_cf(:)
      call cFMS_set_current_interp(interp_id)
      if(C_ASSOCIATED(i_dst)) then
        call C_F_POINTER(i_dst, i_dst_cf, shape=(/current_interp%nxgrid/))
        i_dst_cf = current_interp%i_dst
      end if
      NULLIFY(i_dst_cf)
  end subroutine cFMS_get_i_dst

  subroutine cFMS_get_j_dst(interp_id, j_dst) bind(C, name="cFMS_get_j_dst")
      integer, intent(in) :: interp_id
      type(c_ptr), intent(in), value :: j_dst
      integer, pointer :: j_dst_cf(:)
      call cFMS_set_current_interp(interp_id)
      if(C_ASSOCIATED(j_dst)) then
        call C_F_POINTER(j_dst, j_dst_cf, shape=(/current_interp%nxgrid/))
        j_dst_cf = current_interp%j_dst
      end if
      NULLIFY(j_dst_cf)
  end subroutine cFMS_get_j_dst

  subroutine cFMS_get_version(interp_id, version) bind(C, name="cFMS_get_version")
      integer, intent(in) :: interp_id
      integer, intent(out) :: version
      call cFMS_set_current_interp(interp_id)
      version = current_interp%version
  end subroutine cFMS_get_version

  subroutine cFMS_get_nxgrid(interp_id, nxgrid) bind(C, name="cFMS_get_nxgrid")
      integer, intent(in) :: interp_id
      integer, intent(out) :: nxgrid
      call cFMS_set_current_interp(interp_id)
      nxgrid = current_interp%nxgrid
  end subroutine cFMS_get_nxgrid

  subroutine cFMS_get_nlon_src(interp_id, nlon_src) bind(C, name="cFMS_get_nlon_src")
      integer, intent(in) :: interp_id
      integer, intent(out) :: nlon_src
      call cFMS_set_current_interp(interp_id)
      nlon_src = current_interp%nlon_src
  end subroutine cFMS_get_nlon_src

  subroutine cFMS_get_nlat_src(interp_id, nlat_src) bind(C, name="cFMS_get_nlat_src")
      integer, intent(in) :: interp_id
      integer, intent(out) :: nlat_src
      call cFMS_set_current_interp(interp_id)
      nlat_src = current_interp%nlat_src
  end subroutine cFMS_get_nlat_src

  subroutine cFMS_get_nlon_dst(interp_id, nlon_dst) bind(C, name="cFMS_get_nlon_dst")
      integer, intent(in) :: interp_id
      integer, intent(out) :: nlon_dst
      call cFMS_set_current_interp(interp_id)
      nlon_dst = current_interp%nlon_dst
  end subroutine cFMS_get_nlon_dst

  subroutine cFMS_get_nlat_dst(interp_id, nlat_dst) bind(C, name="cFMS_get_nlat_dst")
      integer, intent(in) :: interp_id
      integer, intent(out) :: nlat_dst
      call cFMS_set_current_interp(interp_id)
      nlat_dst = current_interp%nlat_dst
  end subroutine cFMS_get_nlat_dst


  subroutine cFMS_get_interp_method(interp_id, interp_method) bind(C, name="cFMS_get_interp_method")
      integer, intent(in) :: interp_id
      integer, intent(out) :: interp_method
      call cFMS_set_current_interp(interp_id)
      interp_method = current_interp%interp_method
  end subroutine cFMS_get_interp_method


#include "c_horiz_interp_new.fh"
#include "c_get_interp.fh"
  
end module c_horiz_interp_mod
