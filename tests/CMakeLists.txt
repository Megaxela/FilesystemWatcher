cmake_minimum_required(VERSION 3.9)
project(FilesystemWatcherTests)

set(CMAKE_CXX_STANDARD 17)

if (EXISTS googletest/CMakeLists.txt)
    add_subdirectory(googletest)
endif()

add_executable(FilesystemWatcherTests
    tests.cpp
    Functions.hpp
    filesystem.hpp
)

target_link_libraries(FilesystemWatcherTests
    FilesystemWatcher
    gtest
    stdc++fs
)