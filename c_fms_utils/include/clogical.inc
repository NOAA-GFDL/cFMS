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

function clogical(cbool_in)
  implicit none
  logical(c_bool), intent(in) :: cbool_in
  logical :: clogical
  clogical = logical(cbool_in)
end function clogical
