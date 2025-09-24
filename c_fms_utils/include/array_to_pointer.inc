subroutine CFMS_ARRAY_TO_POINTER_(f_array, f_shape, c_pointer, convert_f_to_c_order)

  implicit none
  integer, intent(in) :: f_shape(:)
  CFMS_ARRAY_TO_POINTER_TYPE_, intent(in) :: f_array(CFMS_ARRAY_TO_POINTER_ASSUMED_SHAPE_)
  type(c_ptr), value, intent(in) :: c_pointer
  logical(c_bool), intent(in), optional :: convert_f_to_c_order

  logical :: convert_f_to_c_order_ = .true.  
  CFMS_ARRAY_TO_POINTER_TYPE_, pointer :: cf_pointer(CFMS_ARRAY_TO_POINTER_ASSUMED_SHAPE_)
  CFMS_ARRAY_TO_POINTER_TYPE_, allocatable :: tmp_array(CFMS_ARRAY_TO_POINTER_ASSUMED_SHAPE_)

  if(present(convert_f_to_c_order)) convert_f_to_c_order_ = logical(convert_f_to_c_order)
  
  if(convert_f_to_c_order_) then
     allocate(tmp_array(CFMS_ARRAY_TO_POINTER_REVERSED_F_SHAPE_))
     tmp_array = reshape(f_array, [CFMS_ARRAY_TO_POINTER_REVERSED_F_SHAPE_], order=[CFMS_ARRAY_TO_POINTER_COLUMN_MAJOR_])
     call c_f_pointer(c_pointer, cf_pointer, [CFMS_ARRAY_TO_POINTER_REVERSED_F_SHAPE_])
     cf_pointer = tmp_array
     deallocate(tmp_array)
  else
     call c_f_pointer(c_pointer, cf_pointer, f_shape)
     cf_pointer = f_array
  end if
  
  nullify(cf_pointer)
  
end subroutine CFMS_ARRAY_TO_POINTER_
