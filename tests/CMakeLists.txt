cmake_minimum_required(VERSION 3.9)
project(FilesystemWatcherTests)

set(CMAKE_CXX_STANDARD 17)

add_executable(FilesystemWatcherTests
    tests.cpp
    Functions.hpp
    filesystem.hpp
)

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/googletest/CMakeLists.txt)
    add_subdirectory(googletest)

    target_include_directories(gtest PUBLIC
        googletest/googletest/include/
    )
endif()

target_link_libraries(FilesystemWatcherTests
    FilesystemWatcher
    gtest
    stdc++fs
)