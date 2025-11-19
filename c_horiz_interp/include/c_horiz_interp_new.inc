function CFMS_HORIZ_INTERP_NEW_2D_(nlon_in, nlat_in, nlon_out, nlat_out, &
   lon_in_ptr, lat_in_ptr, lon_out_ptr, lat_out_ptr, mask_in_ptr, mask_out_ptr, &
   interp_method, verbose, max_dist, src_modulo, is_latlon_in, is_latlon_out, save_weights_as_fregrid, &
   convert_cf_order) bind(C, name=CFMS_HORIZ_INTERP_NEW_2D_BINDC_)

   implicit none

   integer, intent(in) :: nlon_in, nlat_in, nlon_out, nlat_out
   type(c_ptr), intent(in), value :: lon_in_ptr
   type(c_ptr), intent(in), value :: lat_in_ptr
   type(c_ptr), intent(in), value :: lon_out_ptr
   type(c_ptr), intent(in), value :: lat_out_ptr
   type(c_ptr), intent(in), value :: mask_in_ptr
   type(c_ptr), intent(in), value :: mask_out_ptr
   character(c_char), intent(in), optional :: interp_method(MESSAGE_LENGTH)
   integer, intent(in), optional :: verbose
   CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, intent(in), optional :: max_dist
   logical(c_bool), intent(in), optional :: src_modulo
   logical(c_bool), intent(in), optional :: is_latlon_in
   logical(c_bool), intent(in), optional :: is_latlon_out
   logical(c_bool), intent(in), optional :: save_weights_as_fregrid
   logical(c_bool), intent(in), optional :: convert_cf_order

   integer :: CFMS_HORIZ_INTERP_NEW_2D_
   character(MESSAGE_LENGTH-1) :: interp_method_f
   logical :: src_modulo_f
   logical :: is_latlon_in_f
   logical :: is_latlon_out_f
   logical :: save_weights_as_fregrid_f

   integer :: in_shape(2), out_shape(2)

   CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: lon_in(:,:)
   CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: lat_in(:,:)
   CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: lon_out(:,:)
   CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: lat_out(:,:)
   CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: mask_in(:,:)
   CFMS_HORIZ_INTERP_NEW_2D_DATA_TYPE_, allocatable :: mask_out(:,:)

   in_shape = (/nlon_in, nlat_in/)
   out_shape = (/nlon_out, nlat_out/)

   allocate(lon_in(nlon_in+1, nlat_in+1))
   allocate(lat_in(nlon_in+1, nlat_in+1))
   allocate(lon_out(nlon_out+1, nlat_out+1))
   allocate(lat_out(nlon_out+1, nlat_out+1))

   call cFMS_pointer_to_array(lon_in_ptr, in_shape+1, lon_in, convert_cf_order)
   call cFMS_pointer_to_array(lat_in_ptr, in_shape+1, lat_in, convert_cf_order)
   call cFMS_pointer_to_array(lon_out_ptr, out_shape+1, lon_out, convert_cf_order)
   call cFMS_pointer_to_array(lat_out_ptr, out_shape+1, lat_out, convert_cf_order)

   if(c_associated(mask_in_ptr))  then
      allocate(mask_in(nlon_in, nlat_in))
      call cFMS_pointer_to_array(mask_in_ptr, in_shape, mask_in, convert_cf_order)
   end if
   if(c_associated(mask_out_ptr)) then
      allocate(mask_out(nlon_out, nlat_out))
      call cFMS_pointer_to_array(mask_out_ptr, out_shape, mask_out, convert_cf_order)
   end if

   interp_method_f = "conservative"
   if(present(interp_method)) then
      interp_method_f = fms_string_utils_c2f_string(interp_method)
   endif

   if(present(src_modulo)) then
      src_modulo_f = logical(src_modulo)
   else
      src_modulo_f = .false.
   end if
   if(present(is_latlon_in)) then
      is_latlon_in_f = logical(is_latlon_in)
   else
      is_latlon_in_f = .false.
   end if
   if(present(is_latlon_out)) then
      is_latlon_out_f = logical(is_latlon_out)
   else
      is_latlon_out_f = .false.
   end if

   save_weights_as_fregrid_f = .false.
   if(present(save_weights_as_fregrid)) then
      save_weights_as_fregrid_f = logical(save_weights_as_fregrid)
   end if


   CFMS_HORIZ_INTERP_NEW_2D_ = interp_count

   call fms_horiz_interp_new(Interp=interp(interp_count), lon_in=lon_in, lat_in=lat_in, &
      lon_out=lon_out, lat_out=lat_out, verbose=verbose, &
      interp_method=interp_method_f, max_dist=max_dist, &
      src_modulo=src_modulo_f, mask_in=mask_in, &
      mask_out=mask_out, is_latlon_in=is_latlon_in_f, &
      is_latlon_out=is_latlon_out_f, &
      save_weights_as_fregrid=save_weights_as_fregrid_f)

   interp_count = interp_count + 1

   deallocate(lon_in)
   deallocate(lat_in)
   deallocate(lon_out)
   deallocate(lat_out)
   if(allocated(mask_in))  deallocate(mask_in)
   if(allocated(mask_out)) deallocate(mask_out)

end function CFMS_HORIZ_INTERP_NEW_2D_
