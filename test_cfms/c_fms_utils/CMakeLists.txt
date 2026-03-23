add_executable(test_utils test_utils.F90 test_utils_c.c)
target_link_libraries(test_utils PUBLIC cFMS)
target_include_directories(test_utils PUBLIC ../../c_fms ${CMAKE_BINARY_DIR})