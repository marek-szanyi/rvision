cmake_minimum_required(VERSION 3.5)
project(raspvision)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "--std=gnu++11")

find_package(OpenCV REQUIRED)
find_package(Threads)
find_package(Boost REQUIRED COMPONENTS program_options filesystem)

if (NOT DEFINED RVISION_MAJOR_VERSION)
    set(RVISION_MAJOR_VERSION 1)
endif ()
if (NOT DEFINED RVISION_MINOR_VERSION)
    set(RVISION_MINOR_VERSION 0)
endif ()

set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
add_subdirectory(${EXT_PROJECTS_DIR}/easyloggingpp)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)