subroutine CFMS_GATHER_PELIST_2D_(is, ie, js, je, npes, pelist, array_seg, gather_data_c_shape, gather_data, &
     is_root_pe, ishift, jshift, convert_cf_order) bind(C, name=CFMS_GATHER_PELIST_2D_BINDC_)

  implicit none
  integer, intent(in) :: is, ie, js, je
  integer, intent(in) :: npes
  integer, intent(in) :: pelist(npes)
  type(c_ptr), value, intent(in) :: array_seg
  integer, intent(in) :: gather_data_c_shape(2)
  type(c_ptr), value, intent(in) :: gather_data
  logical(c_bool), intent(in) :: is_root_pe
  integer, intent(in), optional :: ishift
  integer, intent(in), optional :: jshift
  logical(c_bool), intent(in), optional :: convert_cf_order

  CFMS_GATHER_PELIST_2D_TYPE_, allocatable :: array_seg_f(:,:)
  CFMS_GATHER_PELIST_2D_TYPE_, allocatable :: gather_data_f(:,:)
  CFMS_GATHER_PELIST_2D_TYPE_, pointer :: test(:,:)
  integer :: gather_data_f_shape(2)

  allocate(array_seg_f(ie-is+1, je-js+1))
  call cFMS_pointer_to_array(array_seg, (/ie-is+1, je-js+1/), array_seg_f, convert_cf_order)

  gather_data_f_shape = gather_data_c_shape
  if(present(convert_cf_order)) then
     if(convert_cf_order) gather_data_f_shape = gather_data_c_shape(2:1:-1)
  end if
  
  if(is_root_pe) then
     allocate(gather_data_f(gather_data_f_shape(1), gather_data_f_shape(2)))
     call cfms_pointer_to_array(gather_data, gather_data_f_shape, gather_data_f, convert_cf_order)
  else
     allocate(gather_data_f(1,1)) !dummy
  end if

  call fms_mpp_gather(is+1, ie+1, js+1, je+1, pelist, array_seg_f, gather_data_f, logical(is_root_pe), ishift, jshift)

  if(is_root_pe) then
     call cFMS_array_to_pointer(gather_data_f, gather_data_f_shape, gather_data, convert_cf_order)
  end if
  
  deallocate(gather_data_f)
  
end subroutine CFMS_GATHER_PELIST_2D_
