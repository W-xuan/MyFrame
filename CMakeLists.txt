cmake_minimum_required(VERSION 3.0)
project(MyFrame)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11 ")
#set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(LIB_SRC
    src/log.cc
    src/util.cc
    src/config.cc
    )

add_library(MyFrame SHARED ${LIB_SRC})

add_executable(test01 tests/test.cc)
add_dependencies(test01 MyFrame)
target_link_libraries(test01 MyFrame)

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config MyFrame)
target_link_libraries(test_config MyFrame)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)