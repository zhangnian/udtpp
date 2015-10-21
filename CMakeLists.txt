cmake_minimum_required(VERSION 2.8)

# 项目信息
project (demo)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 查找当前目录下的所有源文件,并将名称保存到 DEMO_SRCS 变量
aux_source_directory(. DEMO_SRCS)


include_directories(/devcomm/udt/include)
include_directories(/data/udtpp_svn/trunk/lib)

LINK_DIRECTORIES(/usr/local/lib)
LINK_DIRECTORIES(/usr/lib64)
LINK_DIRECTORIES(/devcomm/udt/lib)
LINK_DIRECTORIES(/data/udtpp_svn/trunk/lib)

# 指定生成目标
add_executable(demo ${DEMO_SRCS})

target_link_libraries(demo /usr/lib64/libpthread.so /data/udtpp_svn/trunk/lib/libudtpp.a libudt.a)
