#include "AuxiliaryFunctions.h"
#include <iostream>
#include <algorithm>

void naberezhnov::invalidCommand(std::wostream& out)
{
  out << "INVALID COMMAND\n";
}

void naberezhnov::invalidArgument(std::wostream& out)
{
  out << "INVALID ARGUMENT\n";
}

void naberezhnov::notOpened(std::wostream& out)
{
  out << "DICTIONARY ISN'T OPENED\n";
}

void naberezhnov::closeDict(std::wostream& out)
{
  out << "CLOSE DICTIONARY\n";
}

void naberezhnov::createTwoDicts(std::wostream& out)
{
  out << "CREATE AT LEAST 2 DICTIONARIES\n";
}

bool naberezhnov::isWord(const std::wstring& word)
{
  return std::all_of(word.cbegin(), word.cend(), ::isalpha);
}

bool naberezhnov::isTranslate(const std::wstring& translate)
{
  return std::all_of(translate.cbegin(), translate.cend(), [](const wchar_t& letter)
    {
      return (letter >= L'À' && letter <= L'ÿ' ? true : false);
    }
  );
}

std::vector< naberezhnov::Dictionary >::const_iterator naberezhnov::find(const std::vector<Dictionary>& d, const std::wstring& name)
{
  return std::find_if(d.cbegin(), d.cend(), [&name](const Dictionary& dict)
    {
      return (dict.name_ == name ? true : false);
    }
  );
}

bool naberezhnov::exists(const std::vector<Dictionary>& dicts, const std::wstring& name)
{
  bool exists = false;
  exists = std::any_of(dicts.cbegin(), dicts.cend(), [&name](const Dictionary& dict)
    {
      return (dict.name_ == name ? true : false);
    }
  );
  return exists;
}

void naberezhnov::output(const pair& pair, std::wostream& out)
{
  out << pair.first << " - ";
  std::for_each(pair.second.cbegin(), pair.second.cend(), [&out](const std::wstring& translate)
    {
      out << translate << " ";
    }
  );
  out << '\n';
}
