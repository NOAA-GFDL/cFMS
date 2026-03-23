add_executable(test_create_xgrid test_create_xgrid.c)

target_link_libraries(test_create_xgrid PUBLIC cFMS)
target_include_directories(test_create_xgrid PUBLIC 
    ../../c_constants 
    ../../c_grid_utils 
    ../../c_horiz_interp 
    ../../c_fms 
    ../c_fms
)

add_executable(test_horiz_interp_new ../c_fms/c_mpp_domains_helper.c test_horiz_interp_new.c)
target_link_libraries(test_horiz_interp_new PUBLIC cFMS)
target_include_directories(test_horiz_interp_new PUBLIC 
    ../../c_constants 
    ../../c_grid_utils
    ../../c_horiz_interp
    ../../c_fms
    ../c_fms
)

add_executable(test_horiz_interp_base test_horiz_interp_base.c)
target_link_libraries(test_horiz_interp_base PUBLIC cFMS)
target_include_directories(test_horiz_interp_base PUBLIC 
    ../../c_constants 
    ../../c_grid_utils
    ../../c_horiz_interp
    ../../c_fms
    ../c_fms
)

