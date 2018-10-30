cmake_minimum_required(VERSION 3.9)
project(FilesystemWatcher)

set(CMAKE_CXX_STANDARD 17)

option(FSWATCHER_BUILD_TESTS    "Build tests"    Off)
option(FSWATCHER_BUILD_EXAMPLES "Build examples" Off)

if(WIN32)
    add_definitions(-DOS_WINDOWS)
elseif(UNIX)
    add_definitions(-DOS_LINUX)
elseif(APPLE)
    add_definitions(-DOS_OSX)
endif()

if (${FSWATCHER_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()

if (${FSWATCHER_BUILD_TESTS})
    add_subdirectory(tests)
endif()

set(INCLUDE_FILES
    include/AbstractFilesystemWatcher.hpp
    include/LinuxFilesystemWatcher.hpp
    include/BaseFilesystemWatcher.hpp
    include/ManualFilesystemWatcher.hpp
)

set(SOURCE_FILES
    src/AbstractFilesystemWatcher.cpp
    src/LinuxFilesystemWatcher.cpp
    src/BaseFilesystemWatcher.cpp
    src/ManualFilesystemWatcher.cpp)

add_definitions(-DOS_LINUX)

add_library(FilesystemWatcher STATIC
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

target_include_directories(FilesystemWatcher PUBLIC
        include
)