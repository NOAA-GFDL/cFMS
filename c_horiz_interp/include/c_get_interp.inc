subroutine CFMS_GET_INTERP_NAME_(interp_id, i_src, j_src, i_dst, j_dst, &
    area_frac_dst, version, nxgrid, nlon_src, nlat_src, nlon_dst, nlat_dst, &
    is_allocated, interp_method) bind(C, name=CFMS_GET_INTERP_BINDC_)

    ! Current interface only supports CONSERVATIVE + BILINEAR interp methods

    integer, intent(in) :: interp_id
    type(c_ptr), intent(in), value :: i_src
    type(c_ptr), intent(in), value :: j_src
    type(c_ptr), intent(in), value :: i_dst
    type(c_ptr), intent(in), value :: j_dst
    type(c_ptr), intent(in), value :: area_frac_dst
    integer, intent(out), optional :: version
    integer, intent(out), optional :: nxgrid
    integer, intent(out), optional :: nlon_src                  
    integer, intent(out), optional :: nlat_src                  
    integer, intent(out), optional :: nlon_dst                  
    integer, intent(out), optional :: nlat_dst
    logical(c_bool), intent(out), optional :: is_allocated
    integer, intent(out), optional :: interp_method
    

    integer, pointer :: i_src_cf(:)
    integer, pointer :: j_src_cf(:)
    integer, pointer :: i_dst_cf(:)
    integer, pointer :: j_dst_cf(:)
    integer :: id

    ! only for bilinear
    CFMS_GET_INTERP_DATA_TYPE_, pointer :: area_frac_dst_cf(:)

    if(C_ASSOCIATED(i_src)) then
        call C_F_POINTER(i_src, i_src_cf, shape=(/interp(interp_id)%nxgrid/))
        i_src_cf = interp(interp_id)%i_src
    end if

    if(C_ASSOCIATED(j_src)) then
        call C_F_POINTER(j_src, j_src_cf, shape=(/interp(interp_id)%nxgrid/))
        j_src_cf = interp(interp_id)%j_src
    end if

    if(C_ASSOCIATED(i_dst)) then
        call C_F_POINTER(i_dst, i_dst_cf, shape=(/interp(interp_id)%nxgrid/))
        i_dst_cf = interp(interp_id)%i_dst
    end if

    if(C_ASSOCIATED(j_dst)) then
        call C_F_POINTER(j_dst, j_dst_cf, shape=(/interp(interp_id)%nxgrid/))
        j_dst_cf = interp(interp_id)%j_dst
    end if

    if(C_ASSOCIATED(area_frac_dst)) then
        call C_F_POINTER(area_frac_dst, area_frac_dst_cf, shape=(/interp(interp_id)%nxgrid/))
        area_frac_dst_cf = interp(interp_id)%CFMS_GET_INTERP_OBJ_DATA_TYPE_%area_frac_dst
    end if
         
    
    if(PRESENT(version)) version = interp(interp_id)%version

    if(PRESENT(nxgrid)) nxgrid = interp(interp_id)%nxgrid

    if(PRESENT(nlon_src)) nlon_src = interp(interp_id)%nlon_src

    if(PRESENT(nlat_src)) nlat_src = interp(interp_id)%nlat_src
    
    if(PRESENT(nlon_dst)) nlon_dst = interp(interp_id)%nlon_dst
    
    if(PRESENT(nlat_dst)) nlat_dst = interp(interp_id)%nlat_dst

    if(PRESENT(is_allocated)) is_allocated = interp(interp_id)%CFMS_GET_INTERP_OBJ_DATA_TYPE_%is_allocated
    
    if(PRESENT(interp_method)) interp_method = interp(interp_id)%interp_method

    NULLIFY(i_src_cf)
    NULLIFY(j_src_cf)
    NULLIFY(i_dst_cf)
    NULLIFY(j_dst_cf)
    NULLIFY(area_frac_dst_cf)

end subroutine CFMS_GET_INTERP_NAME_


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

subroutine CFMS_GET_WTI_(interp_id, wti) bind(C, name=CFMS_GET_WTI_BINDC_)
    integer, intent(in) :: interp_id
    CFMS_GET_INTERP_DATA_TYPE_, intent(out) :: wti(interp(interp_id)%nlon_dst,interp(interp_id)%nlat_dst,2)
    wti = interp(interp_id)%CFMS_GET_INTERP_OBJ_DATA_TYPE_%wti
end subroutine CFMS_GET_WTI_

subroutine CFMS_GET_WTJ_(interp_id, wtj) bind(C, name=CFMS_GET_WTJ_BINDC_)
    integer, intent(in) :: interp_id
    CFMS_GET_INTERP_DATA_TYPE_, intent(out) :: wtj(interp(interp_id)%nlon_dst,interp(interp_id)%nlat_dst,2)
    wtj = interp(interp_id)%CFMS_GET_INTERP_OBJ_DATA_TYPE_%wtj
end subroutine CFMS_GET_WTJ_
