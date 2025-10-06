!***********************************************************************
!*                   GNU Lesser General Public License
!*
!* This file is part of the GFDL Flexible Modeling System (FMS).
!*
!* FMS is free software: you can redistribute it and/or modify it under
!* the terms of the GNU Lesser General Public License as published by
!* the Free Software Foundation, either version 3 of the License, or (at
!* your option) any later version.
!*
!* FMS is distributed in the hope that it will be useful, but WITHOUT
!* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
!* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
!* for more details.
!*
!* You should have received a copy of the GNU Lesser General Public
!* License along with FMS.  If not, see <http://www.gnu.org/licenses/>.
!***********************************************************************
module c_fms_mod

  use FMS, only : FATAL, NOTE, WARNING
  use FMS, only : FmsMppDomain2D, FmsMppDomainsNestDomain_type
  use FMS, only : fms_init, fms_end, fms_mpp_domains_init
  use FMS, only : fms_string_utils_c2f_string, fms_string_utils_f2c_string, fms_string_utils_cstring2cpointer
  
  use FMS, only : fms_mpp_declare_pelist, fms_mpp_error, fms_mpp_get_current_pelist
  use FMS, only : fms_mpp_npes, fms_mpp_pe, fms_mpp_set_current_pelist

  use FMS, only : fms_mpp_gather
  
  use FMS, only : fms_mpp_domains_define_domains, fms_mpp_domains_define_io_domain, fms_mpp_domains_define_layout
  use FMS, only : fms_mpp_domains_define_nest_domains, fms_mpp_domains_domain_is_initialized
  use FMS, only : fms_mpp_domains_get_compute_domain, fms_mpp_domains_get_data_domain, fms_mpp_domains_get_domain_name
  use FMS, only : fms_mpp_domains_get_layout, fms_mpp_domains_get_pelist
  use FMS, only : fms_mpp_domains_set_compute_domain, fms_mpp_domains_set_data_domain, fms_mpp_domains_set_global_domain
  use FMS, only : fms_mpp_domains_update_domains
  use FMS, only : fms_mpp_domains_define_mosaic

  use FMS, only : THIRTY_DAY_MONTHS, GREGORIAN, JULIAN, NOLEAP
  use FMS, only : fms_time_manager_init, fms_time_manager_set_calendar_type

  use FMS, only : GLOBAL_DATA_DOMAIN, BGRID_NE, CGRID_NE, DGRID_NE, AGRID
  use FMS, only : FOLD_SOUTH_EDGE, FOLD_NORTH_EDGE, FOLD_WEST_EDGE, FOLD_EAST_EDGE

  use FMS, only : CYCLIC_GLOBAL_DOMAIN, NUPDATE,EUPDATE, XUPDATE, YUPDATE
  use FMS, only : NORTH, NORTH_EAST, EAST, SOUTH_EAST, CORNER, CENTER
  use FMS, only : SOUTH, SOUTH_WEST, WEST, NORTH_WEST
  use FMS, only : CYCLIC_GLOBAL_DOMAIN

  use c_fms_utils_mod, only : cFMS_pointer_to_array, cFMS_array_to_pointer
  
  use iso_c_binding

  implicit none

  public :: cFMS_init
  public :: cFMS_end, cFMS_error
  public :: cFMS_get_domain_count, cFMS_get_nest_domain_count
  public :: cFMS_declare_pelist, cFMS_get_current_pelist, cFMS_npes, cFMS_pe, cFMS_set_current_pelist
  public :: cFMS_define_domains
  public :: cFMS_define_io_domain
  public :: cFMS_define_layout
  public :: cFMS_define_nest_domains
  public :: cFMS_domain_is_initialized
  public :: cFMS_gather_pelist_2d_cfloat, cFMS_gather_pelist_2d_cdouble
  public :: cFMS_get_compute_domain
  public :: cFMS_get_domain
  public :: cFMS_get_data_domain
  public :: cFMS_get_domain_from_id
  public :: cFMS_get_domain_name
  public :: cFMS_get_layout
  public :: cFMS_set_compute_domain
  public :: cFMS_set_current_domain
  public :: cFMS_set_data_domain
  public :: cFMS_set_global_domain
  public :: cFMS_define_cubic_mosaic
  
  integer, public, parameter :: NAME_LENGTH = 64 !< value taken from mpp_domains
  integer, public, parameter :: MESSAGE_LENGTH=128
  character(NAME_LENGTH), parameter :: input_nml_path="./input.nml"

  integer, public, bind(C, name="NOTE")    :: NOTE_C    = NOTE
  integer, public, bind(C, name="WARNING") :: WARNING_C = WARNING
  integer, public, bind(C, name="FATAL")   :: FATAL_C   = FATAL

  integer, public, bind(C, name="GLOBAL_DATA_DOMAIN") :: GLOBAL_DATA_DOMAIN_C = GLOBAL_DATA_DOMAIN
  integer, public, bind(C, name="BGRID_NE") :: BGRID_NE_C = BGRID_NE
  integer, public, bind(C, name="CGRID_NE") :: CGRID_NE_C = CGRID_NE
  integer, public, bind(C, name="DGRID_NE") :: DGRID_NE_C = DGRID_NE
  integer, public, bind(C, name="AGRID") :: AGRID_C = AGRID
  integer, public, bind(C, name="FOLD_SOUTH_EDGE") :: FOLD_SOUTH_EDGE_C = FOLD_SOUTH_EDGE
  integer, public, bind(C, name="FOLD_NORTH_EDGE") :: FOLD_NORTH_EDGE_C = FOLD_NORTH_EDGE
  integer, public, bind(C, name="FOLD_WEST_EDGE")  :: FOLD_WEST_EDGE_C = FOLD_WEST_EDGE
  integer, public, bind(C, name="FOLD_EAST_EDGE")  :: FOLD_EAST_EDGE_C = FOLD_EAST_EDGE
  integer, public, bind(C, name="CYCLIC_GLOBAL_DOMAIN") :: CYCLIC_GLOBAL_DOMAIN_C = CYCLIC_GLOBAL_DOMAIN
  integer, public, bind(C, name="NUPDATE") :: NUPDATE_C = NUPDATE
  integer, public, bind(C, name="EUPDATE") :: EUPDATE_C = EUPDATE
  integer, public, bind(C, name="XUPDATE") :: XUPDATE_C = XUPDATE
  integer, public, bind(C, name="YUPDATE") :: YUPDATE_C = YUPDATE
  integer, public, bind(C, name="NORTH") :: NORTH_C = NORTH
  integer, public, bind(C, name="NORTH_EAST") :: NORTH_EAST_C = NORTH_EAST
  integer, public, bind(C, name="EAST")  :: EAST_C = EAST
  integer, public, bind(C, name="SOUTH_EAST") :: SOUTH_EAST_C = SOUTH_EAST
  integer, public, bind(C, name="CORNER") :: CORNER_C = CORNER
  integer, public, bind(C, name="CENTER") :: CENTER_C = CENTER
  integer, public, bind(C, name="SOUTH") :: SOUTH_C = SOUTH
  integer, public, bind(C, name="SOUTH_WEST") :: SOUTH_WEST_C = SOUTH_WEST  
  integer, public, bind(C, name="WEST")  :: WEST_C = WEST
  integer, public, bind(C, name="NORTH_WEST") :: NORTH_WEST_C = NORTH_WEST

  integer, public, bind(C, name="THIRTY_DAY_MONTHS") :: THIRTY_DAY_MONTHS_C = THIRTY_DAY_MONTHS
  integer, public, bind(C, name="GREGORIAN")         :: GREGORIAN_C = GREGORIAN
  integer, public, bind(C, name="JULIAN")            :: JULIAN_C    = JULIAN
  integer, public, bind(C, name="NOLEAP")            :: NOLEAP_C    = NOLEAP

  type(FmsMppDomain2D), allocatable, target, public :: domain(:)  
  type(FmsMppDomain2D), pointer  :: current_domain
  integer :: domain_count

  type(FmsMppDomainsNestDomain_type), allocatable, target, public :: nest_domain(:)
  type(FmsMppDomainsNestDomain_type), pointer :: current_nest_domain
  integer :: nest_domain_count

contains

  !> cFMS_end
  subroutine cFMS_end() bind(C, name="cFMS_end")
    implicit none
    call fms_end()

    if(allocated(domain)) deallocate(domain)
    if(allocated(nest_domain)) deallocate(nest_domain)
    
  end subroutine cFMS_end

  !> cFMS_get_domain_count
  function cFMS_get_domain_count() bind(C, name="cFMS_get_domain_count")
    integer :: cFMS_get_domain_count
    cFMS_get_domain_count = domain_count    
  end function cFMS_get_domain_count

  !> cFMS_get_nest_domain_count
  function cFMS_get_nest_domain_count() bind(C, name="cFMS_get_nest_domain_count")
    integer :: cFMS_get_nest_domain_count
    cFMS_get_nest_domain_count = nest_domain_count    
  end function cFMS_get_nest_domain_count
  
  !> cfms_init
  subroutine cFMS_init(localcomm, alt_input_nml_path, ndomain, nnest_domain, calendar_type) bind(C,  name="cFMS_init")
    
    implicit none
    integer, intent(in), optional :: localcomm
    integer, intent(in), optional :: ndomain
    integer, intent(in), optional :: nnest_domain
    integer, intent(in), optional :: calendar_type
    character(c_char), intent(in), optional :: alt_input_nml_path(NAME_LENGTH)
    
    character(100) :: alt_input_nml_path_f = input_nml_path
    
    if(present(alt_input_nml_path)) &
         alt_input_nml_path_f = fms_string_utils_c2f_string(alt_input_nml_path)
    
    call fms_init(localcomm=localcomm, alt_input_nml_path=alt_input_nml_path_f)
    call fms_mpp_domains_init()

    call fms_time_manager_init()
    if(present(calendar_type)) then
       call fms_time_manager_set_calendar_type(calendar_type)
    else
       call fms_time_manager_set_calendar_type(NOLEAP)
    end if
    
    if(present(ndomain)) then
       allocate(domain(0:ndomain-1))
    else
       allocate(domain(0:0))
    end if

    if(present(nnest_domain)) then
       allocate(nest_domain(0:nnest_domain-1))
    else
       allocate(nest_domain(0:0))
    end if

    domain_count = 0
    nest_domain_count = 0
    
  end subroutine cfms_init

  !> cFMS_declare_pelist
  subroutine cFMS_declare_pelist(npes, pelist, name, commID) bind(C, name="cFMS_declare_pelist")

    implicit none
    integer, intent(in) :: npes
    integer, intent(in) :: pelist(npes)
    character(c_char), intent(in), optional :: name(NAME_LENGTH)
    integer, intent(out), optional :: commID

    character(len=NAME_LENGTH) :: name_f=" " !mpp default

    if(present(name)) name_f = fms_string_utils_c2f_string(name)
    call fms_mpp_declare_pelist(pelist, name_f, commID)
    
  end subroutine cFMS_declare_pelist
  
  !> cFMS_error
  subroutine cFMS_error(errortype, errormsg) bind(C, name="cFMS_error")
    
    implicit none
    integer, intent(in), value :: errortype
    character(c_char), intent(in), optional :: errormsg(MESSAGE_LENGTH)
    character(len=MESSAGE_LENGTH) :: errormsg_f=""

    if(present(errormsg)) errormsg_f = fms_string_utils_c2f_string(errormsg)
    call fms_mpp_error(errortype, trim(errormsg_f))
    
  end subroutine cFMS_error

  
  !> cFMS_get_current_pelist
  subroutine cFMS_get_current_pelist(npes, pelist, name, commID) bind(C, name="cFMS_get_current_pelist")

    implicit none
    integer, intent(in) :: npes
    integer, intent(out) :: pelist(npes)
    character(c_char), intent(out), optional :: name(NAME_LENGTH)
    integer, intent(out), optional :: commID

    character(len=NAME_LENGTH) :: name_f=" " !mpp default
    
    if(present(name)) name_f = fms_string_utils_c2f_string(name)
    call fms_mpp_get_current_pelist(pelist, name_f, commID)
    
  end subroutine cFMS_get_current_pelist

  !> cFMS_npes
  function cFMS_npes() bind(C, name="cFMS_npes")
    implicit none
    integer :: cFMS_npes
    cFMS_npes = fms_mpp_npes()
  end function cFMS_npes  

  !> cFMS_pes
  function cFMS_pe() bind(C, name="cFMS_pe")
    implicit none
    integer :: cFMS_pe
    cFMS_pe = fms_mpp_pe()
  end function cFMS_pe
  
  !> cFMS_set_current_pelist
  subroutine cFMS_set_current_pelist(npes, pelist, no_sync) bind(C, name="cFMS_set_current_pelist")

    implicit none
    
    integer, intent(in), optional :: npes
    type(c_ptr), intent(in), optional :: pelist
    logical(c_bool), intent(in), optional :: no_sync

    integer, pointer :: pelist_f(:)
    
    if(present(pelist)) then
       if(present(npes)) then
          allocate(pelist_f(npes))
          call c_f_pointer(pelist, pelist_f, (/npes/))
       else
          call fms_mpp_error(FATAL, "cFMS_set_current_pelist, must provide number of pes in the pelist")
       end if
    end if
    if(present(no_sync)) then
       call fms_mpp_set_current_pelist(pelist_f, logical(no_sync))
    else
       !unallocated variables are considered as not-present for optional arguments
       call fms_mpp_set_current_pelist(pelist_f)
    end if
    
  end subroutine cFMS_set_current_pelist
  
  function cFMS_define_domains(global_indices, layout, npes, pelist,            &
       xflags, yflags, xhalo, yhalo, xextent, yextent, maskmap, name,           &
       symmetry, memory_size, whalo, ehalo, shalo, nhalo, is_mosaic, tile_count,&
       tile_id, complete, x_cyclic_offset, y_cyclic_offset) bind(C, name="cFMS_define_domains")

    implicit none   
    integer, intent(inout) :: global_indices(4) 
    integer, intent(in) :: layout(2)
    integer, intent(in) :: npes
    integer, intent(in), optional :: pelist(npes)
    integer, intent(in), optional :: xflags, yflags
    integer, intent(in), optional :: xhalo, yhalo
    integer, intent(in), optional :: xextent(layout(1)), yextent(layout(2))
    logical(c_bool), intent(in), optional :: maskmap(layout(1)*layout(2))
    character(c_char), intent(in), optional :: name(NAME_LENGTH)
    logical(c_bool), intent(in), optional :: symmetry
    integer, intent(in), optional :: memory_size(2)
    integer, intent(in), optional :: whalo, ehalo, shalo, nhalo
    logical(c_bool), intent(in), optional :: is_mosaic
    integer, intent(inout), optional :: tile_count
    integer, intent(inout), optional :: tile_id
    logical(c_bool), intent(in), optional :: complete
    integer, intent(in), optional :: x_cyclic_offset
    integer, intent(in), optional :: y_cyclic_offset
    
    character(len=NAME_LENGTH) :: name_f = "cdomain"
    integer :: global_indices_f(4)
    logical(c_bool) :: maskmap_f(layout(1),layout(2))
    logical :: symmetry_f  = .False.
    logical :: is_mosaic_f = .False.
    logical :: complete_f  = .True.

    integer :: cFMS_define_domains
    
    global_indices_f = global_indices + 1

    if(present(tile_id))    tile_id = tile_id + 1;
    if(present(tile_count)) tile_count = tile_count + 1;
    if(present(name))       name_f = fms_string_utils_c2f_string(name)
    if(present(symmetry))   symmetry_f = logical(symmetry)
    if(present(is_mosaic))  is_mosaic_f = logical(is_mosaic)
    if(present(complete))   complete_f = logical(complete)

    if(present(maskmap)) then
       maskmap_f = reshape(maskmap, layout)
    else
       maskmap_f = .True.
    end if

    cFMS_define_domains = domain_count
    
    call cFMS_set_current_domain(cFMS_define_domains)
    call fms_mpp_domains_define_domains(global_indices_f, layout, current_domain, pelist=pelist,  & 
         xflags=xflags, yflags=yflags, xhalo=xhalo, yhalo=yhalo, xextent=xextent, yextent=yextent,&
         maskmap=logical(maskmap_f), name=name_f, symmetry=symmetry_f, memory_size=memory_size,            &
         whalo=whalo, ehalo=ehalo, shalo=shalo, nhalo=nhalo, is_mosaic=is_mosaic_f, tile_count=tile_count, &
         tile_id=tile_id, complete=complete_f, x_cyclic_offset=x_cyclic_offset, y_cyclic_offset=y_cyclic_offset)

    if(present(tile_id))    tile_id = tile_id - 1;
    if(present(tile_count)) tile_count = tile_count - 1;

    domain_count = domain_count + 1
    
  end function cFMS_define_domains


  !> cFMS_define_io_domain
  subroutine cFMS_define_io_domain(io_layout, domain_id) bind(C, name="cFMS_define_io_domain")
    
    implicit none
    integer, intent(in) :: io_layout(2)
    integer, intent(in) :: domain_id
    
    call cFMS_set_current_domain(domain_id)
    call fms_mpp_domains_define_io_domain(current_domain, io_layout)
    
  end subroutine cFMS_define_io_domain
  

  !> cFMS_define_layout
  subroutine cFMS_define_layout(global_indices, ndivs, layout) bind(C, name="cFMS_define_layout")
    
    implicit none
    integer, intent(in) :: global_indices(4)
    integer, intent(in) :: ndivs
    integer, intent(out) :: layout(2)

    call fms_mpp_domains_define_layout(global_indices, ndivs, layout)

  end subroutine cFMS_define_layout
  

  !> cFMS_define_nest_domain calls mpp_define_nest_domains to define the nest domain with id=nest_domain_id.
  !! Nest_domain_id must be an integer.  This wrapper assumes C indexing convention where array indexing starts from 0  
  function cFMS_define_nest_domains(num_nest, ntiles, nest_level, tile_fine, tile_coarse, &
       istart_coarse, icount_coarse, jstart_coarse, jcount_coarse, npes_nest_tile,        &
       x_refine, y_refine, domain_id, extra_halo, name) bind(C, name="cFMS_define_nest_domains")

    implicit none
    integer, intent(in) :: num_nest
    integer, intent(in) :: ntiles
    integer, intent(in) :: nest_level(num_nest)
    integer, intent(in) :: tile_fine(num_nest)
    integer, intent(in) :: tile_coarse(num_nest)
    integer, intent(in) :: istart_coarse(num_nest)
    integer, intent(in) :: icount_coarse(num_nest)
    integer, intent(in) :: jstart_coarse(num_nest)
    integer, intent(in) :: jcount_coarse(num_nest)
    integer, intent(in) :: npes_nest_tile(ntiles)
    integer, intent(in) :: x_refine(num_nest)
    integer, intent(in) :: y_refine(num_nest)
    integer, intent(in) :: domain_id
    integer, intent(in),  optional :: extra_halo
    character(c_char), intent(in), optional :: name(NAME_LENGTH)

    integer :: istart_coarse_f(num_nest), jstart_coarse_f(num_nest)
    integer :: tile_fine_f(num_nest), tile_coarse_f(num_nest)
    character(100) :: name_f = "cnest_domain"    

    integer :: cFMS_define_nest_domains
    
    istart_coarse_f = istart_coarse + 1
    jstart_coarse_f = jstart_coarse + 1
    tile_fine_f     = tile_fine + 1
    tile_coarse_f   = tile_coarse + 1
    if(present(name)) name_f = fms_string_utils_c2f_string(name)

    cFMS_define_nest_domains = nest_domain_count
    
    call cFMS_set_current_nest_domain(cFMS_define_nest_domains)
    call cFMS_set_current_domain(domain_id)
    call fms_mpp_domains_define_nest_domains(current_nest_domain, current_domain, num_nest, nest_level,             &
         tile_fine_f, tile_coarse_f, istart_coarse_f, icount_coarse, jstart_coarse_f, jcount_coarse, npes_nest_tile,&
         x_refine, y_refine, extra_halo=extra_halo, name=name_f)

    nest_domain_count = nest_domain_count + 1
    
  end function cFMS_define_nest_domains


  function cFMS_domain_is_initialized(domain_id) bind(C, name="cFMS_domain_is_initialized")

    implicit none
    integer, intent(in) :: domain_id
    logical(c_bool) :: cFMS_domain_is_initialized

    call cFMS_set_current_domain(domain_id)
    cFMS_domain_is_initialized = logical(fms_mpp_domains_domain_is_initialized(current_domain), kind=c_bool)

  end function cFMS_domain_is_initialized


  !> cFMS_get_compute_domain
  subroutine cFMS_get_compute_domain(domain_id, xbegin, xend, ybegin, yend, xsize, xmax_size, ysize, ymax_size, &
       x_is_global, y_is_global, tile_count, position, whalo, shalo) bind(C, name="cFMS_get_compute_domain")
    
    implicit none
    integer, intent(in) :: domain_id
    integer, intent(out), optional :: xbegin, xend, ybegin, yend
    integer, intent(out), optional :: xsize, xmax_size, ysize, ymax_size
    logical(c_bool), intent(out), optional :: x_is_global, y_is_global
    integer, intent(inout), optional :: tile_count
    integer, intent(in),  optional :: position
    integer, intent(in),  optional :: whalo, shalo

    integer :: xshift = 0, yshift = 0
    logical :: x_is_global_f, y_is_global_f
    
    if(present(tile_count)) tile_count = tile_count + 1
    if(present(whalo)) xshift = whalo 
    if(present(shalo)) yshift = shalo 
    
    call cFMS_set_current_domain(domain_id)
    call fms_mpp_domains_get_compute_domain(current_domain, xbegin=xbegin, xend=xend, ybegin=ybegin, yend=yend, &
                                            xsize=xsize, xmax_size=xmax_size, ysize=ysize, ymax_size=ymax_size, &
                                            x_is_global=x_is_global_f, y_is_global=y_is_global_f, &
                                            tile_count=tile_count, position=position)

    if(present(xbegin)) xbegin = xbegin + xshift - 1
    if(present(xend))   xend   = xend   + xshift - 1 
    if(present(ybegin)) ybegin = ybegin + yshift - 1 
    if(present(yend))   yend   = yend   + yshift - 1    
    if(present(tile_count)) tile_count = tile_count - 1
    if(present(x_is_global)) x_is_global = logical(x_is_global_f, kind=c_bool)
    if(present(y_is_global)) y_is_global = logical(y_is_global_f, kind=c_bool)
    
  end subroutine cFMS_get_compute_domain


  function cFMS_get_domain(domain_id)
    implicit none
    integer, intent(in), optional :: domain_id
    type(FmsMppDomain2D), pointer :: cFMS_get_domain

    if(present(domain_id)) then
       cFMS_get_domain => domain(domain_id)
    else
       cFMS_get_domain => current_domain
    end if
  end function cFMS_get_domain
    
  !> cFMS_get_data_domain
  subroutine cFMS_get_data_domain(domain_id, xbegin, xend, ybegin, yend, xsize, xmax_size, ysize, ymax_size, &
       x_is_global, y_is_global, tile_count, position, whalo, shalo) bind(C, name="cFMS_get_data_domain")
    
    implicit none
    integer, intent(in) :: domain_id
    integer, intent(out), optional :: xbegin, xend, ybegin, yend
    integer, intent(out), optional :: xsize, xmax_size, ysize, ymax_size
    logical(c_bool), intent(out), optional :: x_is_global, y_is_global
    integer, intent(inout), optional :: tile_count
    integer, intent(in), optional  :: position
    integer, intent(in), optional  :: whalo, shalo

    integer :: xshift = 0, yshift = 0
    logical :: x_is_global_f, y_is_global_f
    
    if(present(tile_count)) tile_count = tile_count + 1
    if(present(whalo)) xshift = whalo
    if(present(shalo)) yshift = shalo    
    
    call cFMS_set_current_domain(domain_id)
    call fms_mpp_domains_get_data_domain(current_domain, xbegin=xbegin, xend=xend, ybegin=ybegin, yend=yend, &
         xsize=xsize, xmax_size=xmax_size, ysize=ysize, ymax_size=ymax_size, x_is_global=x_is_global_f,      &
         y_is_global=y_is_global_f, tile_count=tile_count, position=position)
        
    if(present(xbegin)) xbegin = xbegin + xshift - 1 
    if(present(xend))   xend   = xend   + xshift - 1    
    if(present(ybegin)) ybegin = ybegin + yshift - 1 
    if(present(yend))   yend   = yend   + yshift - 1 
    if(present(tile_count)) tile_count = tile_count - 1
    if(present(x_is_global)) x_is_global = logical(x_is_global,kind=4)
    if(present(y_is_global)) y_is_global = logical(y_is_global,kind=4)
    
  end subroutine cFMS_get_data_domain


  function cFMS_get_domain_from_id(domain_id)
    implicit none
    integer, intent(in) :: domain_id
    type(FmsMppDomain2D), pointer :: cFMS_get_domain_from_id
    cFMS_get_domain_from_id => domain(domain_id)
  end function cFMS_get_domain_from_id
  

  !> cFMS_get_domain_name
  subroutine cFMS_get_domain_name(domain_name_c, domain_id) bind(C, name="cFMS_get_domain_name")
    
    implicit none
    character(c_char), intent(out) :: domain_name_c(NAME_LENGTH)
    integer, intent(in) :: domain_id
    character(len=NAME_LENGTH) :: domain_name_f
    
    call cFMS_set_current_domain(domain_id)
    domain_name_f = fms_mpp_domains_get_domain_name(current_domain)

    call fms_string_utils_f2c_string(domain_name_c, domain_name_f)
    
  end subroutine cFMS_get_domain_name


  subroutine cFMS_get_layout(layout, domain_id) bind(C, name="cFMS_get_layout")
    
    implicit none
    integer, intent(out) :: layout(2)
    integer, intent(in)  :: domain_id

    call cFMS_set_current_domain(domain_id)
    call fms_mpp_domains_get_layout(current_domain, layout)

  end subroutine cFMS_get_layout


  !> cFMS_get_pelist
  subroutine cFMS_get_domain_pelist(npes, pelist, domain_id) bind(C, name="cFMS_get_domain_pelist")
    
    implicit none
    integer, intent(in)  :: npes
    integer, intent(out) :: pelist(npes)
    integer, intent(in)  :: domain_id
    
    call cFMS_set_current_domain(domain_id)
    call fms_mpp_domains_get_pelist(current_domain, pelist)
    
  end subroutine cFMS_get_domain_pelist

    
  !>cFMS_set_compute_domain
  subroutine cFMS_set_compute_domain(domain_id, xbegin, xend, ybegin, yend, xsize, ysize, &
       x_is_global, y_is_global, tile_count, whalo, shalo) bind(C, name="cFMS_set_compute_domain")
    
    implicit none
    integer, intent(in) :: domain_id
    integer, intent(inout), optional :: xbegin, xend, ybegin, yend, xsize, ysize
    logical(c_bool), intent(inout), optional :: x_is_global, y_is_global
    integer, intent(inout), optional :: tile_count
    integer, intent(in),    optional :: whalo, shalo

    integer :: xshift = 0, yshift = 0
    
    if(present(whalo)) xshift = whalo
    if(present(shalo)) yshift = shalo
    
    if(present(xbegin)) xbegin = xbegin - xshift + 1
    if(present(xend))   xend   = xend   - xshift + 1
    if(present(ybegin)) ybegin = ybegin - yshift + 1
    if(present(yend))   yend   = yend   - yshift + 1    
    if(present(tile_count)) tile_count = tile_count + 1

    call cFMS_set_current_domain(domain_id)

    if(present(x_is_global)) then
       if(present(y_is_global)) then
          call fms_mpp_domains_set_compute_domain(domain=current_domain, xbegin=xbegin, xend=xend,&
               tile_count=tile_count, ybegin=ybegin, yend=yend, xsize=xsize, ysize=ysize,         &
               x_is_global=logical(x_is_global), y_is_global=logical(y_is_global))
       else
          call fms_mpp_domains_set_compute_domain(domain=current_domain, xbegin=xbegin, xend=xend,&
               tile_count=tile_count, ybegin=ybegin, yend=yend, xsize=xsize, ysize=ysize,         &
               x_is_global=logical(x_is_global))
       end if
    else if(present(y_is_global)) then
       call fms_mpp_domains_set_compute_domain(domain=current_domain, xbegin=xbegin, xend=xend,&
            tile_count=tile_count, ybegin=ybegin, yend=yend, xsize=xsize, ysize=ysize,         &
            y_is_global=logical(y_is_global))
    else
       call fms_mpp_domains_set_compute_domain(domain=current_domain, xbegin=xbegin, xend=xend,&
            tile_count=tile_count, ybegin=ybegin, yend=yend, xsize=xsize, ysize=ysize)
    end if

    if(present(xbegin)) xbegin = xbegin + xshift - 1
    if(present(xend))   xend   = xend   + xshift - 1
    if(present(ybegin)) ybegin = ybegin + yshift - 1
    if(present(yend))   yend   = yend   + yshift - 1
    if(present(tile_count)) tile_count = tile_count - 1


  end subroutine cFMS_set_compute_domain
 
  
  !> cFMS_set_current_domain sets the domain to the current_domain where the
  !! current_domain has id=domain_id
  subroutine cFMS_set_current_domain(domain_id) bind(C, name="cFMS_set_current_domain")

    implicit none
    integer, intent(in) :: domain_id
    
    current_domain => domain(domain_id)
    
  end subroutine cFMS_set_current_domain


  !> cFMS_set_current_nest_domain sets the nest_domain to the current_nest_domain
  !! where the current_nest_domain has id=nest_domain_id  
  subroutine cFMS_set_current_nest_domain(nest_domain_id)

    implicit none
    integer, intent(in) :: nest_domain_id
    
    current_nest_domain => nest_domain(nest_domain_id)
    
  end subroutine cFMS_set_current_nest_domain


  !> cFMS_set_data_domain
  subroutine cFMS_set_data_domain(domain_id, xbegin, xend, ybegin, yend, xsize, ysize, &
       x_is_global, y_is_global, tile_count, whalo, shalo) bind(C, name="cFMS_set_data_domain")
    
    implicit none
    integer, intent(in) :: domain_id
    integer, intent(inout), optional :: xbegin, xend, ybegin, yend, xsize, ysize
    logical(c_bool), intent(in),    optional :: x_is_global, y_is_global
    integer, intent(inout), optional :: tile_count
    integer, intent(in),    optional :: whalo, shalo
   
    integer :: yshift = 0, xshift = 0
    
    if(present(whalo)) xshift = whalo
    if(present(shalo)) yshift = shalo

    if(present(xbegin)) xbegin = xbegin - xshift + 1
    if(present(xend))   xend   = xend   - xshift + 1
    if(present(ybegin)) ybegin = ybegin - yshift + 1
    if(present(yend))   yend   = yend   - yshift + 1
    if(present(tile_count)) tile_count = tile_count + 1

    !simply way to avoid converting logicals from c_bool to kind=4.  The program will always send in
    ! x_is_global and y_is_global
    call cFMS_set_current_domain(domain_id)

    if(present(x_is_global)) then
       if(present(y_is_global)) then
          call fms_mpp_domains_set_data_domain(current_domain, xbegin=xbegin, xend=xend, ybegin=ybegin, yend=yend, &
               xsize=xsize, ysize=ysize, x_is_global=logical(x_is_global), y_is_global=logical(y_is_global),       &
               tile_count=tile_count)
       else
          call fms_mpp_domains_set_data_domain(current_domain, xbegin=xbegin, xend=xend, ybegin=ybegin, yend=yend, &
               xsize=xsize, ysize=ysize, x_is_global=logical(x_is_global), tile_count=tile_count)
       end if
    else if(present(y_is_global)) then
       call fms_mpp_domains_set_data_domain(current_domain, xbegin=xbegin, xend=xend, ybegin=ybegin, yend=yend, &
            xsize=xsize, ysize=ysize, y_is_global=logical(y_is_global), tile_count=tile_count)
    else
       call fms_mpp_domains_set_data_domain(current_domain, xbegin=xbegin, xend=xend, ybegin=ybegin, yend=yend, &
            xsize=xsize, ysize=ysize, tile_count=tile_count)
    end if

    if(present(xbegin)) xbegin = xbegin + xshift - 1
    if(present(xend))   xend   = xend   + xshift - 1
    if(present(ybegin)) ybegin = ybegin + yshift - 1
    if(present(yend))   yend   = yend   + yshift - 1
    if(present(tile_count)) tile_count = tile_count - 1
    
  end subroutine cFMS_set_data_domain


  !> cFMS_set_global_domain
  subroutine cFMS_set_global_domain(domain_id, xbegin, xend, ybegin, yend, xsize, ysize, tile_count, &
       whalo, shalo) bind(C, name="cFMS_set_global_domain")
    implicit none
    integer, intent(in) :: domain_id
    integer, intent(inout), optional :: xbegin, xend, ybegin, yend, xsize, ysize
    integer, intent(inout), optional :: tile_count
    integer, intent(in),    optional :: whalo, shalo
    
    integer :: xshift = 0, yshift = 0

    if(present(whalo)) xshift = whalo
    if(present(shalo)) yshift = shalo

    if(present(xbegin)) xbegin = xbegin - xshift + 1
    if(present(xend))   xend   = xend   - xshift + 1    
    if(present(ybegin)) ybegin = ybegin - yshift + 1
    if(present(yend))   yend   = yend   - yshift + 1
    if(present(tile_count)) tile_count = tile_count + 1
    
    call cFMS_set_current_domain(domain_id)
    call fms_mpp_domains_set_global_domain(current_domain, xbegin=xbegin, xend=xend, ybegin=ybegin, yend=yend, &
                                           xsize=xsize, ysize=ysize, tile_count=tile_count)

    if(present(xbegin)) xbegin = xbegin + xshift - 1
    if(present(xend))   xend   = xend   + xshift - 1    
    if(present(ybegin)) ybegin = ybegin + yshift - 1
    if(present(yend))   yend   = yend   + yshift - 1
    if(present(tile_count)) tile_count = tile_count - 1
   
  end subroutine cFMS_set_global_domain

  function cFMS_define_cubic_mosaic(ni, nj, global_indices, layout, ntiles, halo, use_memsize) &
       bind(C, name="cFMS_define_cubic_mosaic")
    implicit none
    integer, intent(in) :: ni(6)
    integer, intent(in) :: nj(6)
    integer, intent(in) :: global_indices(4)
    integer, intent(in) :: layout(2)
    integer, intent(in) :: ntiles
    integer, intent(in), optional :: halo
    logical(c_bool), intent(in), optional :: use_memsize

    character(len=NAME_LENGTH) :: type = "Cubic: cubed-sphere"
    logical :: use_memsize_f = .False.
    integer :: n
    integer :: npes_per_tile
    integer :: pe_start(ntiles)
    integer :: pe_end(ntiles)
    integer :: global_indices_f(4)
    integer :: global_indices2d(4,ntiles)
    integer :: layout2d(2,ntiles)
    integer, dimension(12) :: istart1, iend1, jstart1, jend1, tile1
    integer, dimension(12) :: istart2, iend2, jstart2, jend2, tile2
    integer :: num_contact = 12
    integer :: msize(2)
    logical :: use_memsize_local = .True.
    integer :: whalo = 2, shalo = 2, ehalo = 2, nhalo = 2
    integer :: cFMS_define_cubic_mosaic

    global_indices_f = global_indices + 1

    if(present(use_memsize)) then
      use_memsize_f = logical(use_memsize)
      use_memsize_local = use_memsize_f
    end if
    if(present(halo)) then
      nhalo = halo; shalo = halo; whalo = halo; ehalo = halo
    end if

    npes_per_tile = cFMS_npes()/ntiles

    do n = 1, ntiles
      pe_start(n)           = (n-1)*npes_per_tile
      pe_end(n)             = n*npes_per_tile-1
      global_indices2d(:,n) = global_indices_f
      layout2d(:,n)         = layout
    end do

    cFMS_define_cubic_mosaic = domain_count

    call cFMS_set_current_domain(cFMS_define_cubic_mosaic)

    !--- Contact line 1, between tile 1 (EAST) and tile 2 (WEST)
    tile1(1) = 1; tile2(1) = 2
    istart1(1) = ni(1);  iend1(1) = ni(1);  jstart1(1) = 1;      jend1(1) = nj(1)
    istart2(1) = 1;      iend2(1) = 1;      jstart2(1) = 1;      jend2(1) = nj(2)
    !--- Contact line 2, between tile 1 (NORTH) and tile 3 (WEST)
    tile1(2) = 1; tile2(2) = 3
    istart1(2) = 1;      iend1(2) = ni(1);  jstart1(2) = nj(1);  jend1(2) = nj(1)
    istart2(2) = 1;      iend2(2) = 1;      jstart2(2) = nj(3);  jend2(2) = 1
    !--- Contact line 3, between tile 1 (WEST) and tile 5 (NORTH)
    tile1(3) = 1; tile2(3) = 5
    istart1(3) = 1;      iend1(3) = 1;      jstart1(3) = 1;      jend1(3) = nj(1)
    istart2(3) = ni(5);  iend2(3) = 1;      jstart2(3) = nj(5);  jend2(3) = nj(5)
    !--- Contact line 4, between tile 1 (SOUTH) and tile 6 (NORTH)
    tile1(4) = 1; tile2(4) = 6
    istart1(4) = 1;      iend1(4) = ni(1);  jstart1(4) = 1;      jend1(4) = 1
    istart2(4) = 1;      iend2(4) = ni(6);  jstart2(4) = nj(6);  jend2(4) = nj(6)
    !--- Contact line 5, between tile 2 (NORTH) and tile 3 (SOUTH)
    tile1(5) = 2; tile2(5) = 3
    istart1(5) = 1;      iend1(5) = ni(2);  jstart1(5) = nj(2);  jend1(5) = nj(2)
    istart2(5) = 1;      iend2(5) = ni(3);  jstart2(5) = 1;      jend2(5) = 1
    !--- Contact line 6, between tile 2 (EAST) and tile 4 (SOUTH)
    tile1(6) = 2; tile2(6) = 4
    istart1(6) = ni(2);  iend1(6) = ni(2);  jstart1(6) = 1;      jend1(6) = nj(2)
    istart2(6) = ni(4);  iend2(6) = 1;      jstart2(6) = 1;      jend2(6) = 1
    !--- Contact line 7, between tile 2 (SOUTH) and tile 6 (EAST)
    tile1(7) = 2; tile2(7) = 6
    istart1(7) = 1;      iend1(7) = ni(2);  jstart1(7) = 1;      jend1(7) = 1
    istart2(7) = ni(6);  iend2(7) = ni(6);  jstart2(7) = nj(6);  jend2(7) = 1
    !--- Contact line 8, between tile 3 (EAST) and tile 4 (WEST)
    tile1(8) = 3; tile2(8) = 4
    istart1(8) = ni(3);  iend1(8) = ni(3);  jstart1(8) = 1;      jend1(8) = nj(3)
    istart2(8) = 1;      iend2(8) = 1;      jstart2(8) = 1;      jend2(8) = nj(4)
    !--- Contact line 9, between tile 3 (NORTH) and tile 5 (WEST)
    tile1(9) = 3; tile2(9) = 5
    istart1(9) = 1;      iend1(9) = ni(3);  jstart1(9) = nj(3);  jend1(9) = nj(3)
    istart2(9) = 1;      iend2(9) = 1;      jstart2(9) = nj(5);  jend2(9) = 1
    !--- Contact line 10, between tile 4 (NORTH) and tile 5 (SOUTH)
    tile1(10) = 4; tile2(10) = 5
    istart1(10) = 1;     iend1(10) = ni(4); jstart1(10) = nj(4); jend1(10) = nj(4)
    istart2(10) = 1;     iend2(10) = ni(5); jstart2(10) = 1;     jend2(10) = 1
    !--- Contact line 11, between tile 4 (EAST) and tile 6 (SOUTH)
    tile1(11) = 4; tile2(11) = 6
    istart1(11) = ni(4); iend1(11) = ni(4); jstart1(11) = 1;     jend1(11) = nj(4)
    istart2(11) = ni(6); iend2(11) = 1;     jstart2(11) = 1;     jend2(11) = 1
    !--- Contact line 12, between tile 5 (EAST) and tile 6 (WEST)
    tile1(12) = 5; tile2(12) = 6
    istart1(12) = ni(5); iend1(12) = ni(5); jstart1(12) = 1;     jend1(12) = nj(5)
    istart2(12) = 1;     iend2(12) = 1;     jstart2(12) = 1;     jend2(12) = nj(6)
    msize(1) = maxval(ni(:)/layout2d(1,:)) + whalo + ehalo + 1 ! make sure memory domain size is no smaller than
    msize(2) = maxval(nj(:)/layout2d(2,:)) + shalo + nhalo + 1 ! data domain size

    if(use_memsize_local) then
      call fms_mpp_domains_define_mosaic(global_indices2d, layout2d, &
         current_domain, ntiles, num_contact, tile1, tile2, istart1, &
         iend1, jstart1, jend1, istart2, iend2, jstart2, jend2, pe_start, &
         pe_end, symmetry = .true., whalo=whalo, ehalo=ehalo, shalo=shalo, &
         nhalo=nhalo, name=trim(type), memory_size=msize)
    else
      call fms_mpp_domains_define_mosaic(global_indices2d, layout2d, &
      current_domain, ntiles, num_contact, tile1, tile2, istart1, &
      iend1, jstart1, jend1, istart2, iend2, jstart2, jend2, pe_start, &
      pe_end, symmetry = .true., whalo=whalo, ehalo=ehalo, shalo=shalo, &
      nhalo=nhalo, name=trim(type))
    end if
    
    domain_count = domain_count + 1

  end function cFMS_define_cubic_mosaic
  
  !> cFMS_update_domains
#include "c_mpp_gather.fh"
#include "c_update_domains.fh"
#include "c_vector_update_domains.fh"
  
end module c_fms_mod
