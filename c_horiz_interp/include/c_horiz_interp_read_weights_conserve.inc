function cFMS_horiz_interp_read_weights_conserve(weight_filename, weight_file_source, &
     nlon_src, nlat_src, nlon_dst, nlat_dst, isw, iew, jsw, jew, src_tile, save_xgrid_area) &
     bind(C, name="cFMS_horiz_interp_read_weights_conserve")
  
  implicit none
  
  character(c_char), intent(in) :: weight_filename(NAME_LENGTH)
  character(c_char), intent(in) :: weight_file_source(NAME_LENGTH)
  integer, intent(in) :: nlon_src, nlat_src, nlon_dst, nlat_dst
  integer, intent(in) :: isw, iew, jsw, jew
  integer, intent(in) :: src_tile
  logical(c_bool), intent(in), optional :: save_xgrid_area
  
  character(NAME_LENGTH-1) :: weight_filename_f
  character(NAME_LENGTH-1) :: weight_file_source_f
  
  integer :: interp_id
  integer :: cFMS_horiz_interp_read_weights_conserve ! interp id
  
  weight_filename_f = fms_string_utils_c2f_string(weight_filename)
  weight_file_source_f = fms_string_utils_c2f_string(weight_file_source)
  
  interp_id = interp_count
  
  call fms_horiz_interp_read_weights_conserve(interp(interp_id), weight_filename_f, weight_file_source_f, &
       nlon_src, nlat_src, nlon_dst, nlat_dst, isw+1, iew+1, jsw+1, jew+1, src_tile, clogical_f(save_xgrid_area))
  
  cFMS_horiz_interp_read_weights_conserve = interp_id
  
  interp_count = interp_count + 1
  
end function cFMS_horiz_interp_read_weights_conserve
