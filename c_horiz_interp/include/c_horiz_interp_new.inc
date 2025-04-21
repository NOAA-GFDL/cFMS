function CFMS_HORIZ_INTERP_NEW_2D_(lon_in_ptr, lon_in_shape, lat_in_ptr, lat_in_shape, &
    lon_out_ptr, lon_out_shape, lat_out_ptr, lat_out_shape, interp_method, verbose, &
    max_dist, src_modulo, mask_in_ptr, mask_out_ptr, is_latlon_in, is_latlon_out) &
    bind(C, name=CFMS_HORIZ_INTERP_NEW_2D_BINDC_)

  ! Current interface only supports CONSERVATIVE interp method

  implicit none

  type(c_ptr), intent(in), value :: lon_in_ptr
  integer, intent(in) :: lon_in_shape(2)
  type(c_ptr), intent(in), value :: lat_in_ptr
  integer, intent(in) :: lat_in_shape(2)
  type(c_ptr), intent(in), value :: lon_out_ptr
  integer, intent(in) :: lon_out_shape(2)
  type(c_ptr), intent(in), value :: lat_out_ptr
  integer, intent(in) :: lat_out_shape(2)
  type(c_ptr), intent(in), value :: mask_in_ptr
  type(c_ptr), intent(in), value :: mask_out_ptr
  character(c_char), intent(in), optional :: interp_method(MESSAGE_LENGTH)
  integer, intent(in), optional :: verbose
  CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, intent(in), optional :: max_dist
  logical(c_bool), intent(in), optional :: src_modulo
  logical(c_bool), intent(in), optional :: is_latlon_in
  logical(c_bool), intent(in), optional :: is_latlon_out

  integer :: CFMS_HORIZ_INTERP_NEW_2D_
  character(MESSAGE_LENGTH-1) :: interp_method_f
  logical :: src_modulo_f
  logical :: is_latlon_in_f
  logical :: is_latlon_out_f

  CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: lon_in(:,:)
  CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: lat_in(:,:)
  CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: lon_out(:,:)
  CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: lat_out(:,:)
  CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: mask_in(:,:)
  CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: mask_out(:,:)

  allocate(lon_in(lon_in_shape(1), lon_in_shape(2)))
  allocate(lat_in(lat_in_shape(1), lat_in_shape(2)))
  allocate(lon_out(lon_out_shape(1), lon_out_shape(2)))
  allocate(lat_out(lat_out_shape(1), lat_out_shape(2)))

  call cFMS_pointer_to_array(lon_in_ptr, lon_in_shape, lon_in)
  call cFMS_pointer_to_array(lat_in_ptr, lat_in_shape, lat_in)
  call cFMS_pointer_to_array(lon_out_ptr, lon_out_shape, lon_out)
  call cFMS_pointer_to_array(lat_out_ptr, lat_out_shape, lat_out)

  if(c_associated(mask_in_ptr))  then
    allocate(mask_in(lon_in_shape(1), lon_in_shape(2)))
    call cFMS_pointer_to_array(mask_in_ptr, lon_in_shape, mask_in)
  end if
  if(c_associated(mask_out_ptr)) then 
    allocate(mask_out(lon_out_shape(1), lon_out_shape(2)))
    call cFMS_pointer_to_array(mask_out_ptr, lon_out_shape, mask_out)
  end if

  if(present(interp_method) .and. interp_method_f /= '' ) interp_method_f = fms_string_utils_c2f_string(interp_method)

  if(present(src_modulo)) then 
    src_modulo_f = logical(src_modulo)
  else
    src_modulo_f = .FALSE.
  end if
  if(present(is_latlon_in)) then 
    is_latlon_in_f = logical(is_latlon_in)
  else
    is_latlon_in_f = .FALSE.
  end if
  if(present(is_latlon_out)) then
    is_latlon_out_f = logical(is_latlon_out)
  else
    is_latlon_out_f = .FALSE.
  end if

  call fms_horiz_interp_new(Interp=current_interp, lon_in=lon_in, lat_in=lat_in, &
                              lon_out=lon_out, lat_out=lat_out, verbose=verbose, &
                              interp_method=interp_method_f, max_dist=max_dist, &
                              src_modulo=src_modulo_f, mask_in=mask_in, &
                              mask_out=mask_out, is_latlon_in=is_latlon_in_f, &
                              is_latlon_out=is_latlon_out_f)

  
  deallocate(lon_in)
  deallocate(lat_in)
  deallocate(lon_out)
  deallocate(lat_out)
  if(allocated(mask_in))  deallocate(mask_in)
  if(allocated(mask_out)) deallocate(mask_out)

  CFMS_HORIZ_INTERP_NEW_2D_ = current_interp_id

end function CFMS_HORIZ_INTERP_NEW_2D_


  







