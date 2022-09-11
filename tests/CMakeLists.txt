include(FetchContent)
FetchContent_Declare(
        ut
        GIT_REPOSITORY https://github.com/boost-ext/ut.git
        GIT_TAG master
)

set(CMAKE_BUILD_TYPE=Release)
FetchContent_MakeAvailable(ut)
function (test_exec ID SOURCE)
    add_executable(${ID} ${SOURCE})
    target_link_libraries(${ID} PRIVATE nand2tetris Boost::ut fmt::fmt-header-only)
    target_compile_definitions(${ID} PRIVATE ASSEMBLER_TEST_INPUT_DIR="${CMAKE_SOURCE_DIR}/tests/inputs")
    add_test(NAME ${ID} COMMAND ${ID})
endfunction()


test_exec(test_lexer test_lexer.cpp)
test_exec(test_parser test_parser.cpp)
test_exec(test_bit_conversion test_bit_conversion.cpp)
test_exec(test_assembler test_assembler.cpp)
test_exec(test_resolved_writer test_parser.cpp)
target_compile_definitions(test_resolved_writer PRIVATE POST_ASSEMBLER_RUN=True)
