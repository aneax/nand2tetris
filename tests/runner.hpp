
#ifndef MINI_CUSTOM_TEST_RUNNER_HPP
#define MINI_CUSTOM_TEST_RUNNER_HPP
#include <filesystem>
#include <fmt/core.h>
#include <boost/ut.hpp>
template <typename Apply>
static void run(std::string_view path, Apply apply)
{
  namespace fs = std::filesystem;
  if (fs::is_directory(path)) {
    for (auto const& dir_entry : fs::directory_iterator{path}) {
      if (dir_entry.path().extension() == "asm") {
        std::string ipath = dir_entry.path();
        run(ipath, apply);
      }
    }
  } else {
    try {
      fmt::print("Source Path: {}\n", path);
      apply(path);
    } catch (std::runtime_error& error) {
      boost::ut::expect(false) << path;
    }
    //fmt::print("\n\n");
  }
}
#endif