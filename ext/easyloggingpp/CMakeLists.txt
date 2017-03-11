cmake_minimum_required(VERSION 3.5)
project(easylogging_installer C CXX)
include(ExternalProject)


ExternalProject_Add(easylogging
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external
        GIT_REPOSITORY https://github.com/dreal-deps/easyloggingpp.git
        GIT_TAG master
        UPDATE_COMMAND ${GIT_EXECUTABLE} fetch --all
        COMMAND ${GIT_EXECUTABLE} reset --hard origin/master
        CONFIGURE_COMMAND echo "nothing to configure"
        BUILD_COMMAND echo "nothing to build"
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/easyloggingpp && cp -v src/easylogging++.h ${CMAKE_CURRENT_BINARY_DIR}/include/easyloggingpp/)


# Specify include dir
ExternalProject_Get_Property(easylogging source_dir)
set(EASYLOGGINGPP_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/include/easyloggingpp PARENT_SCOPE)


