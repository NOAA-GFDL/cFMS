subroutine cFMS_get_xgrid_area(interp_id, xgrid_area) bind(C, name="cFMS_get_xgrid_area")
    integer, intent(in)         :: interp_id
    real(c_double), intent(out) :: xgrid_area(interp(interp_id)%nxgrid)
    xgrid_area = interp(interp_id)%xgrid_area
end subroutine cFMS_get_xgrid_area
