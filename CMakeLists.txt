cmake_minimum_required(VERSION 3.10)
project(EmbeddedVFS_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Now including both File.cpp and Directory.cpp
add_executable(VFS_Demo 
    src/File.cpp 
    src/Directory.cpp 
    examples/main.cpp
)

if(NOT MSVC)
    target_compile_options(VFS_Demo PRIVATE -Wall -Wextra -Wpedantic)
endif()