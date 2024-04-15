#include "Dictionary.h"
#include <iostream>
#include <algorithm>
#include "AuxiliaryFunctions.h"

naberezhnov::Dictionary::Dictionary(const std::wstring& name):
  dictionary_(map()),
  name_(name)
{}

bool naberezhnov::Dictionary::insert(const std::wstring& word, const std::wstring& translate)
{
  std::pair< map::iterator, bool > inserted = dictionary_.insert(pair(word, std::forward_list< std::wstring >()));
  if (inserted.second == true)
  {
    inserted.first->second.push_front(translate);
    return true;
  }
  else
  {
    if (std::binary_search(inserted.first->second.cbegin(), inserted.first->second.cend(), translate))
    {
      return false;
    }
    inserted.first->second.push_front(translate);
    inserted.first->second.sort();
    return true;
  }
}

bool naberezhnov::Dictionary::search(const std::wstring& word)
{
  return (dictionary_.find(word) != dictionary_.end() ? true : false);
}

bool naberezhnov::Dictionary::deleteKey(const std::wstring& word)
{
  return ((dictionary_.erase(word) != 0) ? true : false);
}

void naberezhnov::Dictionary::editTranslate(const std::wstring& word, const std::wstring& trans, const std::wstring& yaTrans)
{
  auto it = dictionary_.find(word);
  if (it != dictionary_.end())
  {
    std::replace_if(it->second.begin(), it->second.end(), [&trans, &yaTrans](const std::wstring& translate)
      {
        return (translate == trans ? true : false);
      }
    , yaTrans);
  }
}

void naberezhnov::Dictionary::translate(const std::wstring& word, std::wostream& out)
{
  auto it = dictionary_.find(word);
  if (it != dictionary_.end())
  {
    out << it->first << " - ";
    std::for_each(it->second.cbegin(), it->second.cend(), [&out](const std::wstring& translate)
      {
        out << translate << " ";
      }
    );
  }
}

void naberezhnov::Dictionary::printLetter(const char& c, std::wostream& out)
{
  std::for_each(this->dictionary_.cbegin(), this->dictionary_.cend(), [&c, &out](const pair& pair)
    {
      if (pair.first[0] == c)
      {
        output(pair, out);
      }
    }
  );
}

void naberezhnov::Dictionary::print(std::wostream& out)
{
  std::for_each(this->dictionary_.cbegin(), this->dictionary_.cend(), [&out](const pair& pair)
    {
      output(pair, out);
    }
  );
}
