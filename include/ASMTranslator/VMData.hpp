#ifndef NAND2TETRIS_INCLUDE_ASMTRANSLATOR_VMDATA_HPP
#define NAND2TETRIS_INCLUDE_ASMTRANSLATOR_VMDATA_HPP

#include "VMCommand.hpp"
#include <Filesystem.hpp>
#include <filesystem>
#include <vector>
namespace hack::vasm
{
using VMCommandVector = std::vector<VMCommand>;
struct VMData {
    fs::path        source;
    fs::path        dest;
    VMCommandVector commands;

    VMData(const fs::path& src, VMCommandVector cmd)
        : source(src)
        , dest(src)
        , commands(std::move(cmd))
    {
      dest.replace_extension(".asm");
    }
};

}   //namespace hack::vasm

#endif