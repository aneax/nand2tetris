function (test_exec ID SOURCE)
    add_executable(${ID} ${SOURCE})
    target_link_libraries(${ID} PRIVATE utils vasm Boost::ut fmt::fmt-header-only)
    target_compile_definitions(${ID} PRIVATE TEST_BYTECODE_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/inputs")
    add_test(NAME ${ID} COMMAND ${ID})
endfunction()

test_exec(test_vm_parser test_vm_parser.cpp)