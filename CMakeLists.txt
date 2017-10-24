cmake_minimum_required(VERSION 3.9)
project(FilesystemWatcher)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    add_definitions(-DOS_WINDOWS)
elseif(UNIX)
    add_definitions(-DOS_LINUX)
elseif(APPLE)
    add_definitions(-DOS_OSX)
endif()

add_definitions(-DOS_LINUX)

add_executable(FilesystemWatcher
        main.cpp
        AbstractFilesystemWatcher.cpp
        AbstractFilesystemWatcher.hpp
        LinuxFilesystemWatcher.cpp
        LinuxFilesystemWatcher.hpp
        BaseFilesystemWatcher.cpp
        BaseFilesystemWatcher.hpp
        ManualFilesystemWatcher.cpp ManualFilesystemWatcher.hpp)