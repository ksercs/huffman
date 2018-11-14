cmake_minimum_required(VERSION 2.8)

project(HUFFMAN)

include_directories(${HUFFMAN_SOURCE_DIR})

add_executable(MAIN
               util/main.cpp)
add_executable(TESTING
               test/huffman_testing.cpp
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc)

add_library(HUFFMAN
	    lib/huffman.h
	    lib/huffman.cpp
	    lib/my_reader.h
	    lib/my_reader.cpp
	    lib/my_writer.h
	    lib/my_writer.cpp
	    lib/tree.h
	    lib/tree.cpp
        lib/constants.h)

include_directories(lib
	                util
		            test)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(MAIN HUFFMAN -lpthread)
target_link_libraries(TESTING HUFFMAN -lpthread)
