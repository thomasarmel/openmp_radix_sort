cmake_minimum_required(VERSION 3.16)
project(radix_sort)

set(CMAKE_CXX_STANDARD 14)

add_executable(radix_sort main.c config.h config.c radix_sort.h radix_sort.c)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(radix_sort PUBLIC OpenMP::OpenMP_CXX)
endif()
target_link_libraries(radix_sort PUBLIC m)