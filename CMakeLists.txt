cmake_minimum_required(VERSION 3.30)
project(ALotOfPendulums2)

set(CMAKE_CXX_STANDARD 14)

# Set the default build type to Release if not set in CLion
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Set compiler optimization flags for Release and Debug
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")


find_package(Boost REQUIRED)
find_package(OpenCL REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS C CXX)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${HDF5_INCLUDE_DIRS})
link_directories(${HDF5_LIBRARY_DIRS})


add_executable(ALotOfPendulums2 main.cpp)


target_link_libraries(ALotOfPendulums2
        ${OpenCL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${HDF5_LIBRARIES})