cmake_minimum_required(VERSION 2.8) 

if(MSVC)
    set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} /WX:NO /MTd")
else()
	ADD_DEFINITIONS(
	    -std=c++0x
	    -Wall
	    -Wno-unknown-pragmas
	)
endif(MSVC)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

project(buffer)

set(BUFFER_SOURCE_DIR "src")
file(GLOB_RECURSE BUFFER_SOURCES
    "${BUFFER_SOURCE_DIR}/*.cpp"
)

add_library(buffer ${BUFFER_SOURCES})

set(BUFFER_TESTS_DIR "test/tests")
file(GLOB_RECURSE BUFFER_TESTS
    "${BUFFER_TESTS_DIR}/*.cpp"
)

add_subdirectory("lib/gtest")
enable_testing()
include_directories("src" "test")
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CURL_INCLUDE_DIR})
add_executable(runUnitTests ${BUFFER_TESTS})
target_link_libraries(buffer ${CURL_LIBRARY})
target_link_libraries(runUnitTests buffer gtest gtest_main)
add_test(runUnitTests runUnitTests)
