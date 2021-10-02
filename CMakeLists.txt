cmake_minimum_required(VERSION 3.4.1)
project(mat)
# 包含进文件目录 可太好用了
include_directories(include
        D://eigen/eigen-3.4.0/Eigen
        )
# 查找./src目录下的所有源文件，保存到DIR_SRCS中
aux_source_directory(./src DIR_SRCS)
find_package(Threads)

# 用C++11
add_definitions(-std=c++11)
# 指定生成目标文件
add_executable(main ${DIR_SRCS})

target_link_libraries(main ${cmake_THREAD_LIBS_INIT})

