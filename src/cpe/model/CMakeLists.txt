message(STATUS "Adding library model")
add_library(model material.cpp nodelist.cpp property.cpp)
target_include_directories(model PUBLIC ${PROJECT_SOURCE_DIR}/src)

message(STATUS "Adding unit test for material")
add_executable(material_test material.test.cpp)
target_link_libraries(material_test model GTest::gtest_main)
add_test(NAME material_test COMMAND material_test)

message(STATUS "Adding unit test for nodelist")
add_executable(nodelist_test nodelist.test.cpp)
target_link_libraries(nodelist_test model GTest::gtest_main)
add_test(NAME nodelist_test COMMAND nodelist_test)

message(STATUS "Adding unit test for property")
add_executable(property_test property.test.cpp)
target_link_libraries(property_test model GTest::gtest_main)
add_test(NAME property_test COMMAND property_test)
