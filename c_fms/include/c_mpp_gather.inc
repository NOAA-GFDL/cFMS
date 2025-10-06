subroutine CFMS_GATHER_PELIST_2D_(is, ie, js, je, npes, pelist, array_seg, gather_data_c_shape, gather_data, &
     is_root_pe, ishift, jshift, convert_cf_order) bind(C, name=CFMS_GATHER_PELIST_2D_BINDC_)

  implicit none
  integer, intent(in) :: is, ie, js, je
  integer, intent(in) :: npes
  integer, intent(in) :: pelist(npes)
  type(c_ptr), intent(in) :: array_seg
  integer, intent(in) :: gather_data_c_shape(2)
  type(c_ptr), intent(inout) :: gather_data
  logical(c_bool), intent(in) :: is_root_pe
  integer, intent(in), optional :: ishift
  integer, intent(in), optional :: jshift
  logical(c_bool), intent(in), optional :: convert_cf_order

  real(CFMS_GATHER_PELIST_2D_KIND_), pointer :: array_seg_f(:,:)
  real(CFMS_GATHER_PELIST_2D_KIND_), pointer :: gather_data_f(:,:)
  integer :: gather_data_f_shape(2)

  allocate(array_seg_f(ie-is+1, je-js+1))
  call cFMS_pointer_to_array(array_seg, (/ie-is+1, je-js+1/), array_seg_f, convert_cf_order)

  gather_data_f_shape = gather_data_c_shape
  if(present(convert_cf_order)) then
     if(convert_cf_order) gather_data_f_shape = gather_data_c_shape(2:1:-1)
  end if
  
  if(is_root_pe) then
     call cfms_pointer_to_array(gather_data, gather_data_f_shape, gather_data_f, convert_cf_order)
  else
     allocate(gather_data_f(1,1)) !dummy
  end if

  call fms_mpp_gather(is, ie, js, je, pelist, array_seg_f, gather_data_f, logical(is_root_pe), ishift, jshift)

  if(is_root_pe) then
     call cFMS_array_to_pointer(gather_data_f, gather_data_f_shape, gather_data, convert_cf_order)
  end if
  
end subroutine CFMS_GATHER_PELIST_2D_
