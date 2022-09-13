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



add_library(assembler OBJECT)
add_library(vasm OBJECT)
add_library(utils OBJECT)
add_subdirectory(include)
add_subdirectory(src)

add_subdirectory(tools)

# add_library(nand2tetris OBJECT)
add_library(nand2tetris OBJECT $<TARGET_OBJECTS:utils> $<TARGET_OBJECTS:assembler>)
target_link_libraries(nand2tetris PRIVATE fmt::fmt-header-only)
# target_link_libraries(nand2tetris PRIVATE utils vasm assembler)


add_executable(runner main.cpp)
target_link_libraries(runner PRIVATE fmt::fmt-header-only)

option(ENABLE_TESTS "Enable tests" ON)
if (ENABLE_TESTS)
message(STATUS "Enabling Tests")
enable_testing()
add_subdirectory(tests)
endif()