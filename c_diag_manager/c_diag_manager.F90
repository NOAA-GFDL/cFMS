module c_diag_manager_mod

  use FMS, only : fms_diag_init, fms_diag_end
  use FMS, only : fms_diag_register_diag_field, fms_diag_register_static_field, fms_diag_axis_init
  use FMS, only : fms_diag_send_data, fms_diag_send_complete, fms_diag_set_time_end
  use FMS, only : DIAG_OTHER, DIAG_OCEAN, DIAG_ALL

  use fms_diag_yaml_mod, only: get_num_unique_fields

  use FMS, only : fms_string_utils_c2f_string, fms_string_utils_f2c_string  
  
  use FMS, only : FmsTime_type, Operator(+)
  use FMS, only : fms_time_manager_set_date, fms_time_manager_set_time

  use FMS, only : fms_mpp_error, FATAL
  use FMS, only : fms_time_manager_get_date
  
  use FMS, only : FmsMppDomain2D

  use c_fms_mod, only : cFMS_get_domain
  use c_fms_mod, only : NAME_LENGTH, MESSAGE_LENGTH
  use c_fms_utils_mod, only : cFMS_pointer_to_array

  use iso_c_binding
  
  implicit none

  private
  
  public :: cFMS_diag_axis_init_cfloat
  public :: cFMS_diag_axis_init_cdouble  
  public :: cFMS_diag_end
  public :: cFMS_diag_send_complete
  public :: cFMS_diag_init
  public :: cFMS_diag_set_time_end
  public :: cFMS_diag_set_field_init_time
  public :: cFMS_diag_set_field_timestep
  public :: cFMS_diag_advance_field_time
  
  public :: cFMS_register_diag_field_scalar_cint
  public :: cFMS_register_diag_field_scalar_cfloat
  public :: cFMS_register_diag_field_scalar_cdouble
  public :: cFMS_register_diag_field_array_cint
  public :: cFMS_register_diag_field_array_cfloat
  public :: cFMS_register_diag_field_array_cdouble
  
  public :: cFMS_diag_send_data_2d_cint
  public :: cFMS_diag_send_data_3d_cint
  public :: cFMS_diag_send_data_4d_cint
  public :: cFMS_diag_send_data_2d_cfloat
  public :: cFMS_diag_send_data_3d_cfloat
  public :: cFMS_diag_send_data_4d_cfloat
  public :: cFMS_diag_send_data_2d_cdouble
  public :: cFMS_diag_send_data_3d_cdouble
  public :: cFMS_diag_send_data_4d_cdouble
  
  type(FmsTime_type) :: field_init_time
  type(FmsTime_type) :: cFMS_diag_end_time
  type(FmsTime_type), allocatable :: field_curr_time(:)
  type(FmsTime_type), allocatable :: field_timestep(:)
  
  integer, public, bind(C, name="DIAG_OTHER") :: DIAG_OTHER_C = DIAG_OTHER
  integer, public, bind(C, name="DIAG_OCEAN") :: DIAG_OCEAN_C = DIAG_OCEAN
  integer, public, bind(C, name="DIAG_ALL")   :: DIAG_ALL_C   = DIAG_ALL

contains

  subroutine cFMS_diag_end() bind(C, name="cFMS_diag_end")
    implicit none

    call fms_diag_end(cFMS_diag_end_time)
    
  end subroutine cFMS_diag_end

  !cFMS_diag_init
  subroutine cFMS_diag_init(diag_model_subset, time_init, err_msg) bind(C, name='cFMS_diag_init')

    implicit none
    integer, intent(in), optional :: diag_model_subset
    integer, intent(in), optional :: time_init(6)
    character(c_char), intent(out), optional :: err_msg(MESSAGE_LENGTH)

    integer :: nfields    
    character(len=MESSAGE_LENGTH-1) :: err_msg_f = "None"
    
    call fms_diag_init(diag_model_subset = diag_model_subset, &
                       time_init = time_init, &
                       err_msg = err_msg_f)

    nfields = get_num_unique_fields()
    allocate(field_curr_time(nfields))
    allocate(field_timestep(nfields))

    if(present(err_msg) .and. err_msg_f /= '' ) call fms_string_utils_f2c_string(err_msg, err_msg_f)
    
  end subroutine cFMS_diag_init


  !cFMS_diag_send_complete
  subroutine cFMS_diag_send_complete(diag_field_id, err_msg) bind(C, name="cFMS_diag_send_complete")

    implicit none
    integer, intent(in) :: diag_field_id
    character(c_char), intent(out), optional :: err_msg(MESSAGE_LENGTH)

    character(len=MESSAGE_LENGTH-1) :: err_msg_f = "None"
    
    call fms_diag_send_complete(field_timestep(diag_field_id), err_msg_f)

    if(present(err_msg) .and. err_msg_f /= '' ) call fms_string_utils_f2c_string(err_msg, err_msg_f) 
    
  end subroutine cFMS_diag_send_complete
  

  !cFMS_diag_set_field_init_time
  subroutine cFMS_diag_set_field_init_time(year, month, day, hour, minute, second, tick, err_msg) &
       bind(C, name="cFMS_diag_set_field_init_time")

    implicit none
    integer, intent(in) :: year
    integer, intent(in) :: month
    integer, intent(in) :: day
    integer, intent(in) :: hour
    integer, intent(in) :: minute
    integer, intent(in), optional :: second
    integer, intent(in), optional :: tick
    character, intent(out), optional :: err_msg(MESSAGE_LENGTH)

    character(MESSAGE_LENGTH-1) :: err_msg_f = ""
    
    field_init_time = fms_time_manager_set_date(year = year,     &
                                                month = month,   &
                                                day = day,       &
                                                hour = hour,     &
                                                minute = minute, &
                                                second = second, &
                                                tick = tick,     &
                                                err_msg = err_msg_f)

    if(present(err_msg) .and. err_msg_f /= '') call fms_string_utils_f2c_string(err_msg, err_msg_f)
    
  end subroutine cFMS_diag_set_field_init_time


  !cFMS_diag_set_field_timestep
  subroutine cFMS_diag_set_field_timestep(diag_field_id, dseconds, ddays, dticks, &
       err_msg) bind(C, name="cFMS_diag_set_field_timestep")

    implicit none
    integer, intent(in) :: diag_field_id
    integer, intent(in) :: dseconds
    integer, intent(in), optional :: ddays
    integer, intent(in), optional :: dticks
    character, intent(out), optional :: err_msg(MESSAGE_LENGTH)

    character(MESSAGE_LENGTH-1) :: err_msg_f = ""

    field_timestep(diag_field_id) = fms_time_manager_set_time(seconds = dseconds, &
                                                              days = ddays,       &         
                                                              ticks = dticks,     &
                                                              err_msg = err_msg_f)    
    
    if(present(err_msg) .and. err_msg_f /= '') call fms_string_utils_f2c_string(err_msg, err_msg_f)
    
  end subroutine cFMS_diag_set_field_timestep

  
  !cFMS_diag_advance_field_time
  subroutine cFMS_diag_advance_field_time(diag_field_id) bind(C, name="cFMS_diag_advance_field_time")
    
    implicit none
    integer, intent(in) :: diag_field_id

    integer :: year, month, day, hour, minute, second
    
    field_curr_time(diag_field_id) = field_curr_time(diag_field_id) + field_timestep(diag_field_id)

    
  end subroutine cFMS_diag_advance_field_time

  subroutine cFMS_diag_set_time_end(year, month, day, hour, minute, second, tick, err_msg) &
       bind(C, name="cFMS_diag_set_time_end")

    implicit none
    integer, intent(in), optional :: year
    integer, intent(in), optional :: month
    integer, intent(in), optional :: day
    integer, intent(in), optional :: hour
    integer, intent(in), optional :: minute
    integer, intent(in), optional :: second
    integer, intent(in), optional :: tick
    character(c_char), intent(in), optional :: err_msg(MESSAGE_LENGTH)

    character(MESSAGE_LENGTH-1) :: err_msg_f

    cFMS_diag_end_time = fms_time_manager_set_date(year = year,     &
                                                   month = month,   &
                                                   day = day,       &
                                                   hour = hour,     &
                                                   minute = minute, &
                                                   second = second, &
                                                   tick = tick,     &
                                                   err_msg = err_msg_f)

    call fms_diag_set_time_end(cFMS_diag_end_time)
    
  end subroutine cFMS_diag_set_time_end
  
#include "c_diag_axis_init.fh"
#include "c_register_diag_field.fh"
#include "c_send_data.fh"  
  
  !subroutine register_diag_field
  !subroutine register_static_field
  
  
  !subroutine send_data
  !subroutine send_tile_averaged_data
  !subroutine diag_end
  !subroutine get_base_time
  !subroutine get_base_date
  !subroutine get_diag_global_att
  !subroutine set_diag_global_att
  !subroutine diag_field_add_attribute
  !subroutine get_diag_field_id
  !subroutine diag_axis_add_attribute
  !subroutine diag_send_complete
  !subroutine diag_send_complete_instant
  
  
end module c_diag_manager_mod
