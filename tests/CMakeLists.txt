include(FetchContent)
FetchContent_Declare(
        ut
        GIT_REPOSITORY https://github.com/boost-ext/ut.git
        GIT_TAG master
)

set(CMAKE_BUILD_TYPE=Release)
FetchContent_MakeAvailable(ut)

add_subdirectory(Assembler)
add_subdirectory(VMTranslator)