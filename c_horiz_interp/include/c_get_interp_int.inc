subroutine cFMS_get_i_src(interp_id, i_src) bind(C, name="cFMS_get_i_src")
   integer, intent(in) :: interp_id
   integer, intent(out):: i_src(interp(interp_id)%nxgrid)
   i_src = interp(interp_id)%i_src - 1
end subroutine cFMS_get_i_src

subroutine cFMS_get_j_src(interp_id, j_src) bind(C, name="cFMS_get_j_src")
   integer, intent(in) :: interp_id
   integer, intent(out) :: j_src(interp(interp_id)%nxgrid)
   j_src = interp(interp_id)%j_src - 1
end subroutine cFMS_get_j_src

 !check for correctness
subroutine cFMS_get_i_lon(interp_id, i_lon) bind(C, name="cFMS_get_i_lon")
   integer, intent(in) :: interp_id
   integer, intent(out) :: i_lon(interp(interp_id)%nlon_dst, interp(interp_id)%nlat_dst, 2)
   call fms_mpp_error(WARNING, "cFMS_get_i_lon may not have been implemented correctly")
   i_lon = interp(interp_id)%i_lon
end subroutine cFMS_get_i_lon

 !check for correctness
subroutine cFMS_get_j_lat(interp_id, j_lat) bind(C, name="cFMS_get_j_lat")
   integer, intent(in) :: interp_id
   integer :: j_lat(interp(interp_id)%nlon_dst, interp(interp_id)%nlat_dst, 2)
   call fms_mpp_error(WARNING, "cFMS_get_j_lat may not have been implemented correctly")
   j_lat = interp(interp_id)%j_lat
end subroutine cfms_get_j_lat

subroutine cFMS_get_i_dst(interp_id, i_dst) bind(C, name="cFMS_get_i_dst")
   integer, intent(in) :: interp_id
   integer :: i_dst(interp(interp_id)%nxgrid)
   i_dst = interp(interp_id)%i_dst - 1
end subroutine cFMS_get_i_dst

subroutine cFMS_get_j_dst(interp_id, j_dst) bind(C, name="cFMS_get_j_dst")
   integer, intent(in) :: interp_id
   integer :: j_dst(interp(interp_id)%nxgrid)
   j_dst = interp(interp_id)%j_dst - 1
end subroutine cFMS_get_j_dst

subroutine cFMS_get_version(interp_id, version) bind(C, name="cFMS_get_version")
   integer, intent(in) :: interp_id
   integer, intent(out) :: version
   version = interp(interp_id)%version
end subroutine cFMS_get_version

subroutine cFMS_get_nxgrid(interp_id, nxgrid) bind(C, name="cFMS_get_nxgrid")
   integer, intent(in) :: interp_id
   integer, intent(out) :: nxgrid
   nxgrid = interp(interp_id)%nxgrid
end subroutine cFMS_get_nxgrid

subroutine cFMS_get_nlon_src(interp_id, nlon_src) bind(C, name="cFMS_get_nlon_src")
   integer, intent(in) :: interp_id
   integer, intent(out) :: nlon_src
   nlon_src = interp(interp_id)%nlon_src
end subroutine cFMS_get_nlon_src

subroutine cFMS_get_nlat_src(interp_id, nlat_src) bind(C, name="cFMS_get_nlat_src")
   integer, intent(in) :: interp_id
   integer, intent(out) :: nlat_src
   nlat_src = interp(interp_id)%nlat_src
end subroutine cFMS_get_nlat_src

subroutine cFMS_get_nlon_dst(interp_id, nlon_dst) bind(C, name="cFMS_get_nlon_dst")
   integer, intent(in) :: interp_id
   integer, intent(out) :: nlon_dst
   nlon_dst = interp(interp_id)%nlon_dst
end subroutine cFMS_get_nlon_dst

subroutine cFMS_get_nlat_dst(interp_id, nlat_dst) bind(C, name="cFMS_get_nlat_dst")
   integer, intent(in) :: interp_id
   integer, intent(out) :: nlat_dst
   nlat_dst = interp(interp_id)%nlat_dst
end subroutine cFMS_get_nlat_dst

subroutine cFMS_get_interp_method(interp_id, interp_method) bind(C, name="cFMS_get_interp_method")
   integer, intent(in) :: interp_id
   integer, intent(out) :: interp_method
   interp_method = interp(interp_id)%interp_method
end subroutine cFMS_get_interp_method
