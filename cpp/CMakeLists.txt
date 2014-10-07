project(rep-redux)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE "DEBUG")
#set(CMAKE_BUILD_TYPE "RELEASE")

set(PROJECT_SOURCE_DIR "/home/josh/git/rep-redux/cpp")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src")

add_library(state	STATIC	src/state.cpp)
add_library(fsm		STATIC	src/fsm.cpp)
add_library(tree	STATIC	src/decision_tree.cpp)
add_library(emulation	STATIC	src/emulation.cpp)

#add_library(censi	STATIC	src/reduce_censi.cpp)
#add_library(josh	STATIC	src/reduce_josh.cpp)
#add_library(exhaustive	STATIC	src/reduce_josh_exhaustive.cpp)
add_library(graph	STATIC	src/Graph.cpp)
#add_library(exact	STATIC	src/reduce_exact.cpp)


add_executable(rep-redux src/main.cpp)
target_link_libraries(rep-redux graph tree fsm state)
