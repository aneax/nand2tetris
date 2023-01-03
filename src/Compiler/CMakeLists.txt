include(FetchContent)
FetchContent_Declare(
  pegtl
  GIT_REPOSITORY https://github.com/taocpp/PEGTL.git
  GIT_TAG 3.2.7)
FetchContent_MakeAvailable(pegtl)

#FetchContent_Declare(
#  yaml
#  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#  GIT_TAG yaml-cpp-0.7.0)
#FetchContent_MakeAvailable(yaml)

target_sources(compiler PRIVATE Token.cpp Lexer.cpp Parser.cpp)
target_link_libraries(compiler PUBLIC utils taocpp::pegtl)
