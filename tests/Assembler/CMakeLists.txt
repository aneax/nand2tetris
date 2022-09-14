function (test_exec ID SOURCE)
    add_executable(${ID} ${SOURCE})
    target_link_libraries(${ID} PRIVATE assembler utils Boost::ut fmt::fmt-header-only)
    target_compile_definitions(${ID} PRIVATE ASSEMBLER_TEST_INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/inputs")
    add_test(NAME ${ID} COMMAND ${ID})
endfunction()


test_exec(test_lexer test_lexer.cpp)
test_exec(test_parser test_parser.cpp)
test_exec(test_bit_conversion test_bit_conversion.cpp)
test_exec(test_assembler test_assembler.cpp)
test_exec(test_resolved_writer test_parser.cpp)
target_compile_definitions(test_resolved_writer PRIVATE POST_ASSEMBLER_RUN=True)