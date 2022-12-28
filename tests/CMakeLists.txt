include(FetchContent)
FetchContent_Declare(
        ut
        GIT_REPOSITORY https://github.com/boost-ext/ut.git
        GIT_TAG v1.1.9
)

FetchContent_GetProperties(ut)
if (NOT ut_POPULATED)
   set(CMAKE_BUILD_TYPE "Release")
   set(BOOST_UT_ALLOW_CPM_USE OFF CACHE INTERNAL "Disable CPM for boost::ut")
   FetchContent_Populate(ut)
   add_subdirectory(${ut_SOURCE_DIR} ${ut_BINARY_DIR})
endif()

add_subdirectory(Simulator)
add_subdirectory(Assembler)
add_subdirectory(ASMTranslator)
