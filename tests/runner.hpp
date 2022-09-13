
#ifndef MINI_CUSTOM_TEST_RUNNER_HPP
#define MINI_CUSTOM_TEST_RUNNER_HPP
#include "Filesystem.hpp"
#include <fmt/core.h>
#include <boost/ut.hpp>
#include <string_view>
template <bool Check = true, typename Apply>
inline void run(Apply apply, const fs::path& path)
{
  if (fs::is_directory(path)) {
    for (auto const& dir_entry : fs::directory_iterator{path}) {
      fs::path ipath = dir_entry.path();
      run<Check>(apply, ipath);
    }
  } else {
    try {
      if constexpr (Check) {
        if (path.extension() == ".asm") {
          apply(path);
        }
      } else {
        apply(path);
      }
    } catch (std::runtime_error& error) {
      boost::ut::expect(false) << error.what();
    }
    //fmt::print("\n\n");
  }
}

#endif