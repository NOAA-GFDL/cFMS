function CFMS_DIAG_AXIS_INIT_(name, naxis_data, axis_data, units, cart_name, domain_id,    &
     long_name, direction, set_name, edges, aux, req, tile_count, domain_position, not_xy) &
     bind(C, name=CFMS_DIAG_AXIS_INIT_BINDC_)
    
    implicit none
    character(c_char), intent(in) :: name(NAME_LENGTH)
    integer,           intent(in) :: naxis_data
    real(CFMS_AXIS_DATA_KIND_), intent(in)  :: axis_data(naxis_data)
    character(c_char), intent(in)           :: units(NAME_LENGTH)
    character(c_char), intent(in)           :: cart_name(NAME_LENGTH)
    integer,           intent(in), optional :: domain_id
    character(c_char), intent(in), optional :: long_name(NAME_LENGTH)
    character(c_char), intent(in), optional :: set_name(NAME_LENGTH)
    integer,           intent(in), optional :: direction
    integer,           intent(in), optional :: edges
    character(c_char), intent(in), optional :: aux
    character(c_char), intent(in), optional :: req
    integer,           intent(in), optional :: tile_count
    integer,           intent(in), optional :: domain_position
    logical(c_bool),   intent(in), optional :: not_xy

    type(FmsMppDomain2D), pointer :: domain
    
    character(NAME_LENGTH-1) :: name_f
    character(NAME_LENGTH-1) :: units_f
    character(NAME_LENGTH-1) :: cart_name_f
    character(NAME_LENGTH-1) :: set_name_f
    character(NAME_LENGTH-1) :: long_name_f
    character(NAME_LENGTH-1) :: aux_f
    character(NAME_LENGTH-1) :: req_f    
    
    integer :: CFMS_DIAG_AXIS_INIT_

    logical :: not_xy_f

    set_name_f = ""
    aux_f = "none"
    req_f = "none"
    
    name_f      = fms_string_utils_c2f_string(name)
    units_f     = fms_string_utils_c2f_string(units)
    cart_name_f = fms_string_utils_c2f_string(cart_name)
    if(present(set_name))  set_name_f  = fms_string_utils_c2f_string(set_name)
    if(present(long_name)) long_name_f = fms_string_utils_c2f_string(long_name)

    nullify(domain)
    not_xy_f = .False.
    if(present(not_xy)) then
       not_xy_f = logical(not_xy)
    else
       if(.not.present(domain_id)) call fms_mpp_error(FATAL, "must provide domain_id")
       domain => cFMS_get_domain(domain_id)
    end if

    CFMS_DIAG_AXIS_INIT_ = fms_diag_axis_init(name = name_f,           &
                                              array_data = axis_data,  &
                                              units = units_f,         &
                                              cart_name = cart_name_f, &
                                              long_name = long_name_f, &
                                              direction = direction,   &
                                              set_name = set_name_f,   &
                                              edges = edges,           &
                                              Domain2 = domain,        &
                                              aux = aux,               &
                                              req = req,               &
                                              tile_count = tile_count, &
                                              domain_position = domain_position)
    nullify(domain)
    
  end function CFMS_DIAG_AXIS_INIT_



