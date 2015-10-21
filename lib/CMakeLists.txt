cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#include_directories(.)
include_directories(/devcomm/udt/include)

LINK_DIRECTORIES(/devcomm/udt/lib)
LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib64)

aux_source_directory(. LIBUDTPP_SRC)
ADD_LIBRARY(udtpp STATIC ${LIBUDTPP_SRC})
