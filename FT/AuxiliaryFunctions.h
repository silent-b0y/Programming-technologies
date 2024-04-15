#ifndef AUXILIARYFUNCTIONS_H
#define AUXILIARYFUNCTIONS_H

#include <iosfwd>
#include <string>
#include <vector>
#include "Dictionary.h"

namespace naberezhnov
{
  void invalidCommand(std::wostream& out);
  void invalidArgument(std::wostream& out);
  void notOpened(std::wostream& out);
  void closeDict(std::wostream& out);
  void createTwoDicts(std::wostream& out);
  bool isWord(const std::wstring& word);
  bool isTranslate(const std::wstring& translate);
  std::vector< Dictionary >::const_iterator find(const std::vector< Dictionary >& d, const std::wstring& name);
  bool exists(const std::vector< Dictionary >& dicts, const std::wstring& name);
  void output(const pair& pair, std::wostream& out);
}

#endif
