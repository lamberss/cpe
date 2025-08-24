set(TEST_SUFFIX "libcpe_${TEST_SUFFIX}")

message(STATUS "Adding truss system test")
add_executable(truss_${TEST_SUFFIX} truss.cpp)
target_link_libraries(truss_${TEST_SUFFIX} model GTest::gtest_main)
add_test(NAME truss_${TEST_SUFFIX} COMMAND truss_${TEST_SUFFIX})
