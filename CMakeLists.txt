cmake_minimum_required(VERSION 3.0)
project(MyFrame)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -o0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set(LIB_SRC
    src/log.cc
    src/util.cc
    )

add_library(MyFrame SHARED ${LIB_SRC})

add_executable(test01 tests/test.cc)
add_dependencies(test01 MyFrame)
target_link_libraries(test01 MyFrame)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)