cmake_minimum_required(VERSION 3.15)
project(nand2tetris)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fkeep-static-consts")
set(NAND2TETRIS_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")

# if (CMAKE_BUILD_TYPE STREQUAL "Release")
#         message(STATUS "Enabling lto.")
#         set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
# endif()


include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 9.0.0
)
FetchContent_MakeAvailable(fmt)



add_library(assembler OBJECT)
add_library(vasm OBJECT)
add_library(utils OBJECT)
#add_library(compiler OBJECT)
#add_library(mini OBJECT)
add_library(hardware_simulator OBJECT)
#add_library(hardware_simulator_v2 OBJECT)


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
