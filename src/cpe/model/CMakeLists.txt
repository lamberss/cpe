set(TEST_EXE_PREFIX "${TEST_EXE_PREFIX}_model")
set(TEST_NAME_PREFIX "${TEST_NAME_PREFIX}.model")

set(model_sources element.cpp node.cpp material.cpp model.cpp nodelist.cpp property.cpp)

message(STATUS "Adding library: model")
add_library(model ${model_sources})
target_include_directories(model PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(model PUBLIC matrix)

list(APPEND model_sources dof.hpp elementblock.hpp)

list(SORT model_sources)
foreach(source ${model_sources})
  cmake_path(GET source STEM component)
  set(test_name ${TEST_EXE_PREFIX}_${component})
  message(STATUS "Adding test: ${TEST_NAME_PREFIX}.${component}")
  add_executable(${test_name} ${component}.test.cpp)
  target_link_libraries(${test_name} model GTest::gtest_main)
  add_test(NAME ${TEST_NAME_PREFIX}.${component} COMMAND ${test_name})
endforeach()
