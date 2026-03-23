configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-lib.sh.in ${CMAKE_BINARY_DIR}/test-lib.sh @ONLY)

add_subdirectory(c_data_override)
add_subdirectory(c_diag_manager)
add_subdirectory(c_fms)
add_subdirectory(c_fms_utils)
add_subdirectory(c_grid_utils)
add_subdirectory(c_horiz_interp)

file(GLOB_RECURSE TEST_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/*/*.sh)
# leave out scripts that aren't tests
list(FILTER TEST_SCRIPTS EXCLUDE REGEX "check_crashes.*")
list(FILTER TEST_SCRIPTS EXCLUDE REGEX "test-lib*")
list(FILTER TEST_SCRIPTS EXCLUDE REGEX "tap-driver.*")

# add scripts to ctest
foreach (testScript ${TEST_SCRIPTS})
    get_filename_component(testName ${testScript} NAME_WE)
    get_filename_component(testDirNameFull ${testScript} DIRECTORY)
    get_filename_component(testDirName ${testDirNameFull} NAME_WE)
    add_test(NAME ${testName}
            COMMAND ${testScript}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    # all tests are labeled with directory name
    set_property(TEST ${testName} PROPERTY LABELS ${testDirName})
endforeach()

set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")