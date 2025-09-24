function CFMS_DIAG_SEND_DATA_(diag_field_id, field_shape, field_ptr, err_msg, &
     convert_cf_order) bind(C, name=CFMS_DIAG_SEND_DATA_BINDC_)
  
  implicit none
  
  integer, intent(in) :: diag_field_id
  integer, intent(in) :: field_shape(CFMS_DIAG_SEND_DATA_FIELD_NDIM_)
  type(c_ptr), value, intent(in) :: field_ptr
  character(c_bool), intent(out), optional :: err_msg(MESSAGE_LENGTH)
  logical(c_bool), intent(in), optional :: convert_cf_order
  
  character(MESSAGE_LENGTH-1) :: err_msg_f
  logical(c_bool) :: CFMS_DIAG_SEND_DATA_  

  CFMS_DIAG_SEND_DATA_FIELD_TYPE_, allocatable :: field(CFMS_DIAG_SEND_DATA_FIELD_ASSUMED_SHAPE_)
  
  allocate(field(CFMS_DIAG_SEND_DATA_FIELD_SHAPE_))
  call cFMS_pointer_to_array(field_ptr, field_shape, field, convert_cf_order)
  
  CFMS_DIAG_SEND_DATA_ = fms_diag_send_data(diag_field_id = diag_field_id, &
                                       field = field,                      &
                                       time = field_curr_time(diag_field_id), &
                                       err_msg = err_msg_f)

  CFMS_DIAG_SEND_DATA_ = logical(CFMS_DIAG_SEND_DATA_, kind=c_bool)

  if(present(err_msg) .and. err_msg_f /= '' ) call fms_string_utils_f2c_string(err_msg, err_msg_f)
  deallocate(field)

end function CFMS_DIAG_SEND_DATA_
