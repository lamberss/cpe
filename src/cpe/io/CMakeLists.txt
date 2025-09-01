set(TEST_EXE_PREFIX "${TEST_EXE_PREFIX}_io")
set(TEST_NAME_PREFIX "${TEST_NAME_PREFIX}.io")

set(io_sources vtk.cpp)

message(STATUS "Adding library: io")
add_library(io ${io_sources})
target_include_directories(io PUBLIC ${PROJECT_SOURCE_DIR}/src)

list(SORT io_sources)
foreach(source ${io_sources})
  cmake_path(GET source STEM component)
  set(test_name ${TEST_EXE_PREFIX}_${component})
  message(STATUS "Adding test: ${TEST_NAME_PREFIX}.${component}")
  add_executable(${test_name} ${component}.test.cpp)
  target_link_libraries(${test_name} io model GTest::gtest_main)
  add_test(NAME ${TEST_NAME_PREFIX}.${component} COMMAND ${test_name})
endforeach()
