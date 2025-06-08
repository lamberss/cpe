include(util)

set(_project "cpe")
set(_version "0.1.0")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

h1("Project: ${_project} v${_version} (${CMAKE_BUILD_TYPE})")

option(CPE_DO_UNIT_TESTS "Build the unit tests" ON)
message(STATUS "Option CPE_DO_UNIT_TESTS: ${CPE_DO_UNIT_TESTS}")
if(CPE_DO_UNIT_TESTS)
  set(CPE_USE_GOOGLETEST ON)
endif()

option(CPE_USE_GOOGLETEST "Indicate if googletest should be included" OFF)
message(STATUS "Option CPE_USE_GOOGLETEST: ${CPE_USE_GOOGLETEST}")

option(CPE_USE_YAML_CPP "Indicate if yaml-cpp should be included" OFF)
message(STATUS "Option CPE_USE_YAML_CPP: ${CPE_USE_YAML_CPP}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
