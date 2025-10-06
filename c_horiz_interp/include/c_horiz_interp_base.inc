subroutine CFMS_HORIZ_INTERP_BASE_2D_(interp_id, data_in_ptr, data_out_ptr, &
     mask_in_ptr, mask_out_ptr, verbose, missing_value, &
     missing_permit, new_missing_handle, convert_cf_order) bind(C, name=CFMS_HORIZ_INTERP_BASE_2D_BINDC_)
  
  implicit none
  integer, intent(in) :: interp_id
  type(c_ptr), intent(in), value :: data_in_ptr
  type(c_ptr), intent(in), value :: data_out_ptr
  type(c_ptr), intent(in), value :: mask_in_ptr
  type(c_ptr), intent(in), value :: mask_out_ptr
  integer, intent(in), optional :: verbose
  CFMS_HORIZ_INTERP_BASE_2D_TYPE_, intent(in), optional :: missing_value
  integer, intent(in), optional :: missing_permit
  logical(c_bool), intent(in), optional :: new_missing_handle
  logical(c_bool), intent(in), optional :: convert_cf_order

  logical :: new_missing_handle_f
  logical :: mask_in_present, mask_out_present

  integer :: in_shape(2), out_shape(2)
  integer :: nlon_in, nlat_in, nlon_out, nlat_out
  
  CFMS_HORIZ_INTERP_BASE_2D_TYPE_, allocatable :: data_in(:,:)
  CFMS_HORIZ_INTERP_BASE_2D_TYPE_, allocatable :: data_out(:,:)
  CFMS_HORIZ_INTERP_BASE_2D_TYPE_, pointer :: mask_in(:,:)
  CFMS_HORIZ_INTERP_BASE_2D_TYPE_, pointer :: mask_out(:,:)

  nlon_in = interp(interp_id)%nlon_src
  nlat_in = interp(interp_id)%nlat_src
  nlon_out = interp(interp_id)%nlon_dst
  nlat_out = interp(interp_id)%nlat_dst

  in_shape = (/nlon_in, nlat_in/)
  out_shape = (/nlon_out, nlat_out/)

  allocate(data_in(nlon_in, nlat_in))
  allocate(data_out(nlon_out, nlat_out))

  call cFMS_pointer_to_array(data_in_ptr, in_shape, data_in, convert_cf_order)
  call cFMS_pointer_to_array(data_out_ptr, out_shape, data_out, convert_cf_order)
  
  mask_in_present = c_associated(mask_in_ptr)
  mask_out_present = c_associated(mask_out_ptr)
  if(mask_in_present) then
     call cFMS_pointer_to_array(mask_in_ptr, in_shape, mask_in, convert_cf_order)
  end if
  if(mask_out_present) then
     call cFMS_pointer_to_array(mask_out_ptr, out_shape, mask_out, convert_cf_order)
  end if

  if(present(new_missing_handle)) then
     new_missing_handle_f = logical(new_missing_handle)
  else
     new_missing_handle_f = .False.
  end if
  
  if(mask_in_present) then
     if(mask_out_present) then
        call fms_horiz_interp(interp(interp_id), data_in, data_out, mask_in=mask_in, mask_out=mask_out,&
             verbose=verbose, missing_value=missing_value, missing_permit=missing_permit, &
             new_missing_handle=new_missing_handle_f)
     else
        call fms_horiz_interp(interp(interp_id), data_in, data_out, mask_in=mask_in, &
             verbose=verbose, missing_value=missing_value, missing_permit=missing_permit, &
             new_missing_handle=new_missing_handle_f)
     end if
  else
     call fms_horiz_interp(interp(interp_id), data_in, data_out, &
          verbose=verbose, missing_value=missing_value, missing_permit=missing_permit, &
          new_missing_handle=new_missing_handle_f)
  end if

  call cFMS_array_to_pointer(data_out, out_shape, data_out_ptr, convert_cf_order)

end subroutine CFMS_HORIZ_INTERP_BASE_2D_
