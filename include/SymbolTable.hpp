#ifndef NAND2TETRIS_INCLUDE_SYMBOLTABLE_HPP
#define NAND2TETRIS_INCLUDE_SYMBOLTABLE_HPP

#include <string>
#include <unordered_map>
namespace hack
{

class SymbolTable
{
  public:
    using Key   = std::string;
    using Value = size_t;

    auto contains(const Key& key) const noexcept -> bool { return symbols_.contains(key); }
    void insert(const Key& key, Value value) { symbols_.insert({key, value}); }

  private:
    std::unordered_map<Key, Value> symbols_;
};

}   //namespace hack

#endif