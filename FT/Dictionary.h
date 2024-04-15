#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <forward_list>
#include <vector>
#include <string>

namespace naberezhnov
{
  using map = std::map< std::wstring, std::forward_list< std::wstring > >;
  using pair = std::pair< std::wstring, std::forward_list< std::wstring > >;

  struct Dictionary
  {
    Dictionary(const std::wstring&);
    bool insert(const std::wstring&, const std::wstring&);
    bool search(const std::wstring&);
    bool deleteKey(const std::wstring&);
    void editTranslate(const std::wstring&, const std::wstring&, const std::wstring&);
    void translate(const std::wstring&, std::wostream&);
    void printLetter(const char&, std::wostream&);
    void print(std::wostream&);
    map dictionary_;
    std::wstring name_;
  };
}

#endif
