list(APPEND TEST_C_DATA_OVERRIDE_SRC
    test_data_override_2d.c
    test_data_override_3d.c
    test_data_override_scalar.c
)

foreach (testfile ${TEST_C_DATA_OVERRIDE_SRC})
    get_filename_component(testname ${testfile} NAME_WE)
    add_executable(${testname} ${testfile} ../c_fms/c_mpp_domains_helper.c)
    target_link_libraries(${testname} PUBLIC cFMS)
    target_include_directories(${testname} PUBLIC ../c_fms ../../c_fms ../../c_data_override)
endforeach()

add_executable(test_data_override_ongrid_cfms test_data_override_ongrid.F90)
target_link_libraries(test_data_override_ongrid_cfms PUBLIC FMS::fms)