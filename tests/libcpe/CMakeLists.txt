set(TEST_EXE_PREFIX "${TEST_EXE_PREFIX}_libcpe")
set(TEST_NAME_PREFIX "${TEST_NAME_PREFIX}.libcpe")

message(STATUS "Adding test: ${TEST_NAME_PREFIX}.truss")
add_executable(${TEST_EXE_PREFIX}_truss truss.cpp)
target_link_libraries(${TEST_EXE_PREFIX}_truss model GTest::gtest_main)
add_test(NAME ${TEST_NAME_PREFIX}.truss COMMAND ${TEST_EXE_PREFIX}_truss)
