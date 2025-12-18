subroutine CFMS_GET_AREA_FRAC_DST_(interp_id, area_frac_dst) bind(C, name=CFMS_GET_AREA_FRAC_DST_BINDC_)
   integer, intent(in)                     :: interp_id
   CFMS_GET_INTERP_DATA_TYPE_, intent(out) :: area_frac_dst(interp(interp_id)%nxgrid)
   area_frac_dst = interp(interp_id)%CFMS_GET_INTERP_OBJ_DATA_TYPE_%area_frac_dst
end subroutine CFMS_GET_AREA_FRAC_DST_

subroutine CFMS_GET_IS_ALLOCATED_(interp_id, is_allocated) bind(C, name=CFMS_GET_IS_ALLOCATED_BINDC_)
   integer, intent(in) :: interp_id
   logical(c_bool), intent(out) :: is_allocated
   is_allocated = interp(interp_id)%CFMS_GET_INTERP_OBJ_DATA_TYPE_%is_allocated
end subroutine CFMS_GET_IS_ALLOCATED_

!check for correctness
subroutine CFMS_GET_WTI_(interp_id, wti) bind(C, name=CFMS_GET_WTI_BINDC_)
   integer, intent(in) :: interp_id
   CFMS_GET_INTERP_DATA_TYPE_, intent(out) :: wti(interp(interp_id)%nlon_dst,interp(interp_id)%nlat_dst,2)
   call fms_mpp_error(WARNING, "cFMS_get_wti may not have been implemented correctly")
   wti = interp(interp_id)%CFMS_GET_INTERP_OBJ_DATA_TYPE_%wti
end subroutine CFMS_GET_WTI_

!check for correctness
subroutine CFMS_GET_WTJ_(interp_id, wtj) bind(C, name=CFMS_GET_WTJ_BINDC_)
   integer, intent(in) :: interp_id
   CFMS_GET_INTERP_DATA_TYPE_, intent(out) :: wtj(interp(interp_id)%nlon_dst,interp(interp_id)%nlat_dst,2)
   call fms_mpp_error(WARNING, "cFMS_get_wtj may not have been implemented correctly")
   wtj = interp(interp_id)%CFMS_GET_INTERP_OBJ_DATA_TYPE_%wtj
end subroutine CFMS_GET_WTJ_

subroutine CFMS_GET_XGRID_AREA_(interp_id, xgrid_area) bind(C, name=CFMS_GET_XGRID_AREA_BINDC_)
   integer, intent(in) :: interp_id
   CFMS_GET_INTERP_DATA_TYPE_, intent(out) :: xgrid_area(interp(interp_id)%nxgrid)

   if(kind(xgrid_area) == c_float) then
      call fms_mpp_error(WARNING, "xgrid_area  is saved as c_double kind.  Reducing precision to c_float")
      xgrid_area = real(interp(interp_id)%xgrid_area, c_float)
   else
      xgrid_area = interp(interp_id)%xgrid_area
   end if

end subroutine CFMS_GET_XGRID_AREA_
