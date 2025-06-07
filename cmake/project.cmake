include(util)

set(_project "cpe")
set(_version "0.1.0")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

h1("Project: ${_project} v${_version} (${CMAKE_BUILD_TYPE})")

option(CPE_USE_GOOGLETEST "Indicate if googletest should be included" ON)
message(STATUS "Option CPE_USE_GOOGLETEST: ${CPE_USE_GOOGLETEST}")

option(CPE_USE_YAML_CPP "Indicate if yaml-cpp should be included" ON)
message(STATUS "Option CPE_USE_YAML_CPP: ${CPE_USE_YAML_CPP}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
