subroutine CFMS_DATA_OVERRIDE_2D_(gridname, fieldname, data_shape, data, override, is, ie, js, je) &
     bind(C, name=CFMS_DATA_OVERRIDE_2D_BINDC_)

  implicit none
  character(c_char), intent(in) :: gridname(NAME_LENGTH)
  character(c_char), intent(in) :: fieldname(NAME_LENGTH)
  integer,           intent(in) :: data_shape(2)
  type(c_ptr), value,intent(in) :: data
  logical(c_bool),   intent(out), optional :: override
  integer, intent(in), optional :: is
  integer, intent(in), optional :: ie
  integer, intent(in), optional :: js
  integer, intent(in), optional :: je

  character(len=NAME_LENGTH-1) :: gridname_f
  character(len=NAME_LENGTH-1) :: fieldname_f
  logical :: override_f
  
  CFMS_DATA_OVERRIDE_2D_TYPE_, allocatable :: data_f(:,:)

  gridname_f = fms_string_utils_c2f_string(gridname)
  fieldname_f = fms_string_utils_c2f_string(fieldname)

  allocate(data_f(data_shape(1),data_shape(2)))  
  
  call fms_data_override(gridname = gridname_f,   &
                         fieldname = fieldname_f, &
                         data_2D = data_f, &
                         time = data_override_time, &
                         override = override_f,   &
                         is_in = is, &
                         js_in = js, &
                         ie_in = ie, &
                         je_in = je)
  
  call cfms_array_to_pointer(data_f, data_shape, data)  
  deallocate(data_f)
  
  if(present(override)) override = logical(override_f, c_bool)

end subroutine CFMS_DATA_OVERRIDE_2D_
