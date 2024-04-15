#include "Commands.h"
#include <iostream>
#include <algorithm>
#include "AuxiliaryFunctions.h"

naberezhnov::Commands::Commands(std::wistream& in, std::wostream& out, std::vector<Dictionary> dicts):
  in_(in),
  out_(out),
  dicts_(dicts),
  currentDict_(Dictionary(L"")),
  opened_(false)
{}

void naberezhnov::Commands::help()
{
  out_ << "HELP - for more information\n";
  out_ << "CREATE <name> - create new dictionary called \"name\"\n";
  out_ << "LIST - list of existing dictionaries\n";
  out_ << "HASSIMILAR <dict1> <dict2> - show if there are identical words in \"dict1\" and \"dict2\"\n";
  out_ << "ISSIMILAR <dict1> <dict2> - show whether \"dict1\" and \"dict2\" consist of identical words\n";
  out_ << "MERGE <dict1> <dict2> <dict3> - merge <dict1> and <dict2> into a <dict3> without repeating words\n";
  out_ << "OPEN <dict> - open dictionary \"dict\" for editing\n";
  out_ << "INSERT <word> <translate> - insert \"word\" with \"translate\" in dictionary\n";
  out_ << "SEARCH <word> - show if there is the \"word\" in dictionary\n";
  out_ << "DELETEKEY <word> - delete \"word\" from dictionary\n";
  out_ << "CHANGETRANSLATE <word> <translate1> <translate2> - change \"translate1\" in \"word\" to \"translate2\"\n";
  out_ << "TRANSLATE <word> - print \"word\" with its translates\n";
  out_ << "LETTER <n> - print words beginning with the letter \"n\" with their translates\n";
  out_ << "PRINT - print all dictionary\n";
  out_ << "CLOSE - close opened dictionary\n";
}

void naberezhnov::Commands::create()
{
  std::wstring name;
  in_ >> name;
  if (!exists(dicts_, name))
  {
    Dictionary dict(name);
    dicts_.push_back(dict);
  }
  else
  {
    out_ << "Dictionary already exists.\n";
  }
}

void naberezhnov::Commands::list()
{
  if (!opened_)
  {
    std::wostream& out = out_;
    std::for_each(dicts_.cbegin(), dicts_.cend(), [&out](const Dictionary& dict)
      {
        out << dict.name_ << '\n';
      }
    );
  }
  else
  {
    closeDict(out_);
  }
}

void naberezhnov::Commands::hasSimilar()
{
  if (dicts_.size() < 2)
  {
    createTwoDicts(out_);
  }
  else if (!opened_)
  {
    std::wstring dict = L"";
    std::wstring yaDict = L"";
    in_ >> dict;
    in_ >> yaDict;
    if (exists(dicts_, dict) && exists(dicts_, yaDict))
    {
      auto it = find(dicts_, dict);
      auto yaIt = find(dicts_, yaDict);
      bool hasSimilar = false;
      hasSimilar = std::any_of(it->dictionary_.cbegin(), it->dictionary_.cend(), [&yaIt](const pair& pair)
        {
          return (yaIt->dictionary_.find(pair.first) != yaIt->dictionary_.end() ? true : false);
        }
      );
      if (hasSimilar)
      {
        out_ << "Yes, dictionaries have similar words\n";
      }
      else
      {
        out_ << "No, dictionaries haven't similar words\n";
      }
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    closeDict(out_);
  }
}

void naberezhnov::Commands::isSimilar()
{
  if (dicts_.size() < 2)
  {
    createTwoDicts(out_);
  }
  else if (!opened_)
  {
    std::wstring dict = L"";
    std::wstring yaDict = L"";
    in_ >> dict;
    in_ >> yaDict;
    if (exists(dicts_, dict) && exists(dicts_, yaDict))
    {
      auto it = find(dicts_, dict);
      auto yaIt = find(dicts_, yaDict);
      bool isSimilar = false;
      if (it->dictionary_.size() == yaIt->dictionary_.size())
      {
        isSimilar = std::all_of(it->dictionary_.cbegin(), it->dictionary_.cend(), [&yaIt](const pair& pair)
          {
            return (yaIt->dictionary_.find(pair.first) != yaIt->dictionary_.end() ? true : false);
          }
        );
      }
      if (isSimilar)
      {
        out_ << "Yes, dictionaries are similar\n";
      }
      else
      {
        out_ << "No, dictionaries aren't similar\n";
      }
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    closeDict(out_);
  }
}

void naberezhnov::Commands::merge()
{
  if (dicts_.size() < 2)
  {
    createTwoDicts(out_);
  }
  else if (!opened_)
  {
    std::wstring dict = L"";
    std::wstring yaDict = L"";
    std::wstring dictToMerge = L"";
    in_ >> dict;
    in_ >> yaDict;
    in_ >> dictToMerge;
    if (exists(dicts_, dict) && exists(dicts_, yaDict) && !exists(dicts_, dictToMerge))
    {
      Dictionary dictionary(dictToMerge);
      auto it = find(dicts_, dict);
      auto yaIt = find(dicts_, yaDict);
      std::copy_if(
        it->dictionary_.cbegin(),
        it->dictionary_.cend(),
        std::inserter(dictionary.dictionary_, dictionary.dictionary_.begin()),
        [&yaIt](const pair& pair)
        {
          return (yaIt->dictionary_.find(pair.first) == yaIt->dictionary_.end() ? true : false);
        }
      );
      std::copy_if(
        yaIt->dictionary_.cbegin(),
        yaIt->dictionary_.cend(),
        std::inserter(dictionary.dictionary_, dictionary.dictionary_.begin()),
        [&it](const pair& pair)
        {
          return (it->dictionary_.find(pair.first) == it->dictionary_.end() ? true : false);
        }
      );
      dicts_.push_back(dictionary);
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    closeDict(out_);
  }
}

void naberezhnov::Commands::open()
{
  std::wstring name = L"";
  in_ >> name;
  auto it = find(dicts_, name);
  if (it != dicts_.cend())
  {
    currentDict_ = *it;
    opened_ = true;
  }
  else
  {
    out_ << "Dictionary doesn't exist.\n";
  }
}

void naberezhnov::Commands::insert()
{
  if (opened_)
  {
    std::wstring word = L"";
    std::wstring translate = L"";
    in_ >> word;
    in_ >> translate;
    if (isWord(word) && isTranslate(translate))
    {
      currentDict_.insert(word, translate);
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    notOpened(out_);
  }
}

void naberezhnov::Commands::search()
{
  if (opened_)
  {
    std::wstring word = L"";
    in_ >> word;
    if (isWord(word))
    {
      bool found = currentDict_.search(word);
      if (found)
      {
        out_ << "Word is in dictionary.\n";
      }
      else
      {
        out_ << "No such word in dictionary.\n";
      }
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    notOpened(out_);
  }
}

void naberezhnov::Commands::deleteKey()
{
  if (opened_)
  {
    std::wstring word = L"";
    in_ >> word;
    if (isWord(word))
    {
      currentDict_.deleteKey(word);
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    notOpened(out_);
  }
}

void naberezhnov::Commands::editTranslate()
{
  if (opened_)
  {
    std::wstring word = L"";
    std::wstring translate = L"";
    std::wstring yaTranslate = L"";
    in_ >> word;
    in_ >> translate;
    in_ >> yaTranslate;
    if (isWord(word) && isTranslate(translate) && isTranslate(yaTranslate))
    {
      currentDict_.editTranslate(word, translate, yaTranslate);
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    notOpened(out_);
  }
}

void naberezhnov::Commands::translate()
{
  if (opened_)
  {
    std::wstring word = L"";
    in_ >> word;
    if (isWord(word))
    {
      currentDict_.translate(word, out_);
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    notOpened(out_);
  }
}

void naberezhnov::Commands::printLetter()
{
  if (opened_)
  {
    wchar_t c = '0';
    in_ >> c;
    if (in_ && isalpha(c))
    {
      currentDict_.printLetter(c, out_);
    }
    else
    {
      invalidArgument(out_);
    }
  }
  else
  {
    notOpened(out_);
  }
}

void naberezhnov::Commands::print()
{
  if (opened_)
  {
    currentDict_.print(out_);
  }
  else
  {
    notOpened(out_);
  }
}

void naberezhnov::Commands::close()
{
  if (opened_ == true)
  {
    std::wstring name = currentDict_.name_;
    auto it = find(dicts_, name);
    dicts_.erase(it);
    dicts_.push_back(currentDict_);
    opened_ = false;
  }
  else
  {
    notOpened(out_);
  }
}
