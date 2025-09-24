subroutine CFMS_UPDATE_DOMAINS_SUB_NAME_(field_shape, field, domain_id, flags, complete, position, &
     whalo, ehalo, shalo, nhalo, name_c, tile_count, convert_cf_order) bind(C, name=CFMS_UPDATE_DOMAINS_BINDC_)

  implicit none
  integer, intent(in) :: field_shape(CFMS_UPDATE_DOMAINS_FIELD_NDIM_)
  type(c_ptr), intent(in), value :: field
  integer, intent(in), optional :: domain_id
  integer, intent(in), optional :: flags
  logical, intent(in), optional :: complete
  integer, intent(in), optional :: position
  integer, intent(in), optional :: whalo, ehalo, shalo, nhalo
  character(c_char), intent(in), optional :: name_c(NAME_LENGTH)
  integer, intent(inout), optional :: tile_count
  logical(c_bool), intent(in), optional :: convert_cf_order

  CFMS_UPDATE_DOMAINS_FIELD_TYPE_, allocatable :: field_f(CFMS_UPDATE_DOMAINS_FIELD_POINTER_)
  character(len=NAME_LENGTH) :: name_f
  
  if(present(name_c)) name_f = fms_string_utils_c2f_string(name_c)
  if(present(tile_count)) tile_count = tile_count + 1
  
  allocate(field_f(CFMS_UPDATE_DOMAINS_SHAPE_F_))
  call cFMS_pointer_to_array(field, field_shape, field_f, convert_cf_order)
  
  call cFMS_set_current_domain(domain_id)  
  call fms_mpp_domains_update_domains(field=field_f, domain=current_domain, flags=flags, complete=complete, &
       position=position, whalo=whalo, ehalo=ehalo, shalo=shalo, nhalo=nhalo, name=name_f, tile_count=tile_count)

  call cFMS_array_to_pointer(field_f, field_shape, field, convert_cf_order)
  
  deallocate(field_f)

  if(present(tile_count)) tile_count = tile_count - 1

end subroutine CFMS_UPDATE_DOMAINS_SUB_NAME_
