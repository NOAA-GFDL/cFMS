add_executable(test_grid_utils test_grid_utils.c)
target_link_libraries(test_grid_utils PUBLIC cFMS)
target_include_directories(test_grid_utils PUBLIC ../../c_constants ../../c_grid_utils)