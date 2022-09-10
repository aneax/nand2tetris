cmake_minimum_required(VERSION 3.15)
project(nand2tetris)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(NAND2TETRIS_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
)
FetchContent_MakeAvailable(fmt)


add_library(nand2tetris OBJECT)
add_subdirectory(src)
target_include_directories(nand2tetris PUBLIC include)
target_link_libraries(nand2tetris PRIVATE fmt::fmt)


add_executable(runner main.cpp)
target_link_libraries(runner PRIVATE fmt::fmt)

option(ENABLE_TESTS "Enable tests" ON)
if (ENABLE_TESTS)
message(STATUS "Enabling Tests")
enable_testing()
add_subdirectory(tests)
endif()