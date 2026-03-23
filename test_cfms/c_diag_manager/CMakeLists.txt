cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)

add_executable(test_send_data test_send_data.c ../c_fms/c_mpp_domains_helper.c)
target_link_libraries(test_send_data PUBLIC cFMS)
target_include_directories(test_send_data PUBLIC ../c_fms ../../c_fms ../../c_diag_manager)