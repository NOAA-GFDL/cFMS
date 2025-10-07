subroutine CFMS_POINTER_TO_ARRAY_(c_pointer, f_shape, f_array, convert_c_to_f_order)
  
  implicit none
  type(c_ptr), intent(in), value :: c_pointer
  integer, intent(in) :: f_shape(:)
  CFMS_POINTER_TO_ARRAY_TYPE_, intent(out) :: f_array(CFMS_POINTER_TO_ARRAY_ASSUMED_SHAPE_)
  logical(c_bool), intent(in), optional :: convert_c_to_f_order

  logical :: convert_c_to_f_order_ = .true.
  CFMS_POINTER_TO_ARRAY_TYPE_, pointer :: f_pointer(CFMS_POINTER_TO_ARRAY_ASSUMED_SHAPE_)

  if(present(convert_c_to_f_order)) convert_c_to_f_order_ = logical(convert_c_to_f_order)

  if(convert_c_to_f_order_) then
     call c_f_pointer(c_pointer, f_pointer, CFMS_POINTER_TO_ARRAY_REVERSED_F_SHAPE_)
     f_array = reshape(f_pointer, shape=CFMS_POINTER_TO_ARRAY_SHAPE_, order=CFMS_POINTER_TO_ARRAY_ROW_MAJOR_)
  else
     call c_f_pointer(c_pointer, f_pointer, CFMS_POINTER_TO_ARRAY_SHAPE_)
     f_array = f_pointer
     write(*,*) f_pointer
  end if

  nullify(f_pointer)

end subroutine CFMS_POINTER_TO_ARRAY_
