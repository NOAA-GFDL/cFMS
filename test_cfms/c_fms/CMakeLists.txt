list(APPEND TEST_C_FMS_SRC
    test_define_domains.c
    test_getset_domains.c
    test_update_domains.c
    test_vector_update_domains.c
    test_define_cubic_mosaic.c
)

foreach (testfile ${TEST_C_FMS_SRC})
    get_filename_component(testname ${testfile} NAME_WE)
    add_executable(${testname} ${testfile} c_mpp_domains_helper.c)
    target_link_libraries(${testname} PUBLIC cFMS)
    target_include_directories(${testname} PUBLIC ../../c_fms ../c_fms)
endforeach()

list(APPEND C_TYPE
    int
    float
    double
)

foreach (type ${C_TYPE})
    add_executable(test_gather_c${type} c_mpp_domains_helper.c test_gather.c)
    target_compile_definitions(test_gather_c${type} PRIVATE "CFMS_TEST_KIND_=${type}")
    target_compile_definitions(test_gather_c${type} PRIVATE "CFMS_GATHER_1D_=cFMS_gather_1d_c${type}")
    target_compile_definitions(test_gather_c${type} PRIVATE "CFMS_GATHER_PELIST_2D_=cFMS_gather_pelist_2d_c${type}")
    target_compile_definitions(test_gather_c${type} PRIVATE "CFMS_GATHERV_1D_=cFMS_gatherv_1d_c${type}")
    target_link_libraries(test_gather_c${type} PUBLIC cFMS)
    target_include_directories(test_gather_c${type} PUBLIC ../../c_fms ../c_fms)
endforeach()

add_executable(test_cfms test_cfms.c)
target_link_libraries(test_cfms PUBLIC cFMS)
target_include_directories(test_cfms PUBLIC ../../c_fms)