function (test_exec ID SOURCE)
    add_executable(${ID} ${SOURCE})
    target_link_libraries(${ID} PRIVATE Boost::ut fmt::fmt-header-only)
    add_test(NAME ${ID} COMMAND ${ID})
endfunction()

