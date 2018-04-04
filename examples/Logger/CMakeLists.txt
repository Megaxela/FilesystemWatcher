cmake_minimum_required(VERSION 3.9)
project(LoggerExample)

set(CMAKE_CXX_STANDARD 11)

add_executable(LoggerExample
    main.cpp
)

target_link_libraries(LoggerExample
    FilesystemWatcher
)