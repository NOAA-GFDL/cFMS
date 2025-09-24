subroutine test_3d_cdouble(nx, ny, nz, c_pointer, convert_cf_order) bind(C, name="test_3d_cdouble")
  
  use c_fms_mod, only: cFMS_error, FATAL
  use c_fms_utils_mod, only : cFMS_pointer_to_array, cFMS_array_to_pointer
  use iso_c_binding  
  
  implicit none
  integer, intent(in) :: nx, ny, nz
  type(c_ptr), value, intent(in) :: c_pointer
  logical(c_bool), intent(in) :: convert_cf_order
  
  integer :: i, j, k
  real(c_double), allocatable :: f_array(:,:,:)
  real(c_double), allocatable :: answers(:,:,:)
  
  allocate(answers(nx, ny, nz))
  do k=1, nz
     do j=1, ny
        do i=1, nx
           answers(i,j,k) = (i-1)*100+(j-1)*10+(k-1)
        end do
     end do
  end do

  allocate(f_array(nx, ny, nz))
  call cFMS_pointer_to_array(c_pointer, (/nx, ny, nz/), f_array, convert_cf_order)

  if(any(answers.ne.f_array)) then
     write(*,*) "ERROR converting pointer to array"
     call cFMS_error(FATAL)
  end if

  f_array = - f_array

  call cFMS_array_to_pointer(f_array, (/nx, ny, nz/), c_pointer, convert_cf_order)

  deallocate(f_array)
  deallocate(answers)
  
end subroutine test_3d_cdouble
  
