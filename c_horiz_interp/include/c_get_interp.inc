subroutine CFMS_GET_INTERP_NAME_(interp_id, i_src, j_src, i_dst, j_dst, &
    area_frac_dst, version, nxgrid, nlon_src, nlat_src, nlon_dst, nlat_dst, &
    is_allocated, interp_method) bind(C, name=CFMS_GET_INTERP_BINDC_)

    ! Current interface only supports CONSERVATIVE interp method

    integer, intent(in), optional :: interp_id
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
    CFMS_GET_INTERP_DATA_TYPE_, pointer :: area_frac_dst_cf(:)

    call cFMS_set_current_interp(interp_id)

    if(C_ASSOCIATED(i_src)) then
        call C_F_POINTER(i_src, i_src_cf, shape=(/current_interp%nxgrid/))
        i_src_cf = current_interp%i_src
    end if

    if(C_ASSOCIATED(j_src)) then
        call C_F_POINTER(j_src, j_src_cf, shape=(/current_interp%nxgrid/))
        j_src_cf = current_interp%j_src
    end if

    if(C_ASSOCIATED(i_dst)) then
        call C_F_POINTER(i_dst, i_dst_cf, shape=(/current_interp%nxgrid/))
        i_dst_cf = current_interp%i_dst
    end if

    if(C_ASSOCIATED(j_dst)) then
        call C_F_POINTER(j_dst, j_dst_cf, shape=(/current_interp%nxgrid/))
        j_dst_cf = current_interp%j_dst
    end if

    if(C_ASSOCIATED(area_frac_dst)) then
        call C_F_POINTER(area_frac_dst, area_frac_dst_cf, shape=(/current_interp%nxgrid/))
        area_frac_dst_cf = current_interp%CFMS_GET_INTERP_OBJ_DATA_TYPE_%area_frac_dst
    end if

    if(PRESENT(version)) version = current_interp%version

    if(PRESENT(nxgrid)) nxgrid = current_interp%nxgrid

    if(PRESENT(nlon_src)) nlon_src = current_interp%nlon_src

    if(PRESENT(nlat_src)) nlat_src = current_interp%nlat_src
    
    if(PRESENT(nlon_dst)) nlon_dst = current_interp%nlon_dst
    
    if(PRESENT(nlat_dst)) nlat_dst = current_interp%nlat_dst

    if(PRESENT(is_allocated)) is_allocated = current_interp%CFMS_GET_INTERP_OBJ_DATA_TYPE_%is_allocated
    
    if(PRESENT(interp_method)) interp_method = current_interp%interp_method

    NULLIFY(i_src_cf)
    NULLIFY(j_src_cf)
    NULLIFY(i_dst_cf)
    NULLIFY(j_dst_cf)
    NULLIFY(area_frac_dst_cf)

end subroutine CFMS_GET_INTERP_NAME_


