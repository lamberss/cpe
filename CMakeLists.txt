cmake_minimum_required(VERSION 3.15...4.0 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(project)

h2("SETUP COMPILERS")
message(STATUS "The CMake version is ${CMAKE_VERSION}")
message(STATUS "The CMake generator is \"${CMAKE_GENERATOR}\"")
project("${_project}"
	VERSION "${_version}"
	LANGUAGES C CXX)

include(tpls)

h2("CONFIGURING CODE")
add_subdirectory(src)

h2("FINALIZING")
