set(TEST_EXE_PREFIX "${TEST_EXE_PREFIX}_matrix")
set(TEST_NAME_PREFIX "${TEST_NAME_PREFIX}.matrix")

set(matrix_sources matrix.cpp)

message(STATUS "Adding library: matrix")
add_library(matrix ${matrix_sources})
target_include_directories(matrix PUBLIC ${PROJECT_SOURCE_DIR}/src)

list(SORT matrix_sources)
foreach(source ${matrix_sources})
  cmake_path(GET source STEM component)
  set(test_name ${TEST_EXE_PREFIX}_${component})
  message(STATUS "Adding test: ${TEST_NAME_PREFIX}.${component}")
  add_executable(${test_name} ${component}.test.cpp)
  target_link_libraries(${test_name} matrix GTest::gtest_main)
  add_test(NAME ${TEST_NAME_PREFIX}.${component} COMMAND ${test_name})
endforeach()
