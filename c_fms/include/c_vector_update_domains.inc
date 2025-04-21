subroutine CFMS_V_UPDATE_DOMAINS_SUB_NAME_(fieldx_shape, fieldx, fieldy_shape, fieldy, domain_id, flags, gridtype, &
  complete, whalo, ehalo, shalo, nhalo, name_c, tile_count) bind(C, name=CFMS_V_UPDATE_DOMAINS_BINDC_)

  implicit none
  integer, intent(in) :: fieldx_shape(CFMS_V_UPDATE_DOMAINS_FIELD_NDIM_)
  type(c_ptr), intent(in), value :: fieldx
  integer, intent(in) :: fieldy_shape(CFMS_V_UPDATE_DOMAINS_FIELD_NDIM_)
  type(c_ptr), intent(in), value :: fieldy
  integer, intent(in), optional :: domain_id
  integer, intent(in), optional :: flags
  integer, intent(in), optional :: gridtype
  logical, intent(in), optional :: complete
  integer, intent(in), optional :: whalo, ehalo, shalo, nhalo
  character(c_char), intent(in), optional :: name_c(NAME_LENGTH)
  integer, intent(inout), optional :: tile_count

  CFMS_V_UPDATE_DOMAINS_FIELD_TYPE_, allocatable :: fieldx_f(CFMS_V_UPDATE_DOMAINS_FIELD_POINTER_)
  CFMS_V_UPDATE_DOMAINS_FIELD_TYPE_, allocatable :: fieldy_f(CFMS_V_UPDATE_DOMAINS_FIELD_POINTER_)
  character(len=NAME_LENGTH) :: name_f
  
  if(present(name_c)) name_f = fms_string_utils_c2f_string(name_c)
  if(present(tile_count)) tile_count = tile_count + 1
  
  allocate(fieldx_f(CFMS_V_UPDATE_DOMAINS_SHAPEX_F_))
  call cFMS_pointer_to_array(fieldx, fieldx_shape, fieldx_f)

  allocate(fieldy_f(CFMS_V_UPDATE_DOMAINS_SHAPEY_F_))
  call cFMS_pointer_to_array(fieldy, fieldy_shape, fieldy_f)
  
  call cFMS_set_current_domain(domain_id)  
  call fms_mpp_domains_update_domains(fieldx=fieldx_f, fieldy=fieldy_f, domain=current_domain, flags=flags, &
    gridtype=gridtype, complete=complete, whalo=whalo, ehalo=ehalo, shalo=shalo, nhalo=nhalo, name=name_f, &
    tile_count=tile_count)

  call cFMS_array_to_pointer(fieldx_f, fieldx_shape, fieldx)
  call cFMS_array_to_pointer(fieldy_f, fieldy_shape, fieldy)
  
  deallocate(fieldx_f)
  deallocate(fieldy_f)

  if(present(tile_count)) tile_count = tile_count - 1

end subroutine CFMS_V_UPDATE_DOMAINS_SUB_NAME_