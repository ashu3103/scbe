cmake_minimum_required(VERSION 3.20)

option(BUILD_TESTS "Enable building tests" OFF)

project(scbe)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(scbe STATIC ${LIB_SOURCES})

target_precompile_headers(scbe PRIVATE include/pch.hpp)

target_include_directories(scbe PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)

set_source_files_properties(
  src/codegen/coff_object_emitter.cpp
  PROPERTIES
  COMPILE_FLAGS "-w"
)

install(TARGETS scbe)

# if(BUILD_TESTS)
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

file(GLOB_RECURSE TEST_NEW_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(scbe-test ${TEST_NEW_SOURCES})
target_link_libraries(scbe-test PRIVATE Catch2::Catch2WithMain scbe)

target_include_directories(scbe-test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# endif()