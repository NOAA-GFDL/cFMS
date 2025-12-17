program test_clogical

  use c_fms_utils_mod, only: clogical_f, clogical_t, clogical
  use iso_c_binding
  implicit none
  
  logical(c_bool) :: argument
  logical :: returned
  integer :: lsize

  lsize = sizeof(returned)

  argument = logical(.true., c_bool)
  
  if(sizeof(clogical_t(NULL())).ne.lsize) error stop "incorrect return size"
  if(sizeof(clogical_f(NULL())).ne.lsize) error stop "incorrect return size"
  if(sizeof(clogical(argument)).ne.lsize) error stop "incorrect return size"
  
  returned = clogical_t(NULL()) !returns true
  if(.not.returned) error stop "fail test clogical_t()"

  returned = clogical_f(NULL()) !returns false
  if(returned) error stop "fail test clogical_f()"

  returned = clogical(argument) !returns true
  if(.not.returned) error stop "fail test clogical(argument)"

end program test_clogical
