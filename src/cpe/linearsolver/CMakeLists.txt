set(TEST_EXE_PREFIX "${TEST_EXE_PREFIX}_linearsolver")
set(TEST_NAME_PREFIX "${TEST_NAME_PREFIX}.linearsolver")

set(linearsolver_sources gaussseidel.cpp jacobi.cpp ssor.cpp)

message(STATUS "Adding library: linearsolver")
add_library(linearsolver ${linearsolver_sources})
target_include_directories(linearsolver PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(linearsolver PUBLIC matrix)

list(SORT linearsolver_sources)
foreach(source ${linearsolver_sources})
  cmake_path(GET source STEM component)
  set(test_name ${TEST_EXE_PREFIX}_${component})
  message(STATUS "Adding test: ${TEST_NAME_PREFIX}.${component}")
  add_executable(${test_name} ${component}.test.cpp)
  target_link_libraries(${test_name} linearsolver GTest::gtest_main)
  add_test(NAME ${TEST_NAME_PREFIX}.${component} COMMAND ${test_name})
endforeach()
