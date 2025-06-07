message(STATUS "Adding library model")
add_library(model nodelist.cpp)
target_include_directories(model PUBLIC ${PROJECT_SOURCE_DIR}/src)

message(STATUS "Adding unit test for nodelist")
add_executable(nodelist_test nodelist.test.cpp)
target_link_libraries(nodelist_test model GTest::gtest_main)
