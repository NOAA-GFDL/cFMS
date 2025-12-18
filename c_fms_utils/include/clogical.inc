!> clogical*: work around functions to handle boolean/logical types.
!! portable_kinds is not implemented for logicals in FMS
!! all c_bool must be converted to fortran default logical size of 4 bytes

!! clogical_t: if cbool_in is optional argument, return default value of .true. if cbool_in = NULL
function clogical_t(cbool_in)
  implicit none
  logical(c_bool), intent(in), optional :: cbool_in
  logical :: clogical_t

  if(present(cbool_in)) then
     clogical_t = logical(cbool_in)
  else
     clogical_t = .true.
  end if

end function clogical_t

!! clogical_f: if cbool_in is optional argument, return default value of .false. if cbool_in = NULL
function clogical_f(cbool_in)
  implicit none
  logical(c_bool), intent(in), optional :: cbool_in
  logical :: clogical_f

  if(present(cbool_in)) then
     clogical_f = logical(cbool_in)
  else
     clogical_f = .false.
  end if

end function clogical_f

!! clogical: return logical(cbool_in) for mandatory argument cbool_in
function clogical(cbool_in)
  implicit none
  logical(c_bool), intent(in) :: cbool_in
  logical :: clogical
  clogical = logical(cbool_in)
end function clogical
