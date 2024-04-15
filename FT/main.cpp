#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iterator>
#include <functional>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#elif __linux__
#include <clocale>
#endif
#include "Dictionary.h"
#include "Commands.h"
#include "AuxiliaryFunctions.h"

int main()
{
#ifdef _WIN32
  _setmode(_fileno(stdout), _O_U16TEXT);
  _setmode(_fileno(stdin), _O_U16TEXT);
#elif __linux__
  setlocale(LC_ALL, "en_US.UTF-8");
#endif
  using namespace naberezhnov;
  std::wcout << "Type \"HELP\" for more information.\n";
  std::vector< Dictionary > dictionaries;
  Commands inCommands(std::wcin, std::wcout, dictionaries);
  const std::map< std::wstring, std::function< void() > > commandsMap =
  {
    {L"HELP", std::bind(&naberezhnov::Commands::help, std::ref(inCommands))},
    {L"CREATE", std::bind(&naberezhnov::Commands::create, std::ref(inCommands))},
    {L"LIST", std::bind(&naberezhnov::Commands::list, std::ref(inCommands))},
    {L"HASSIMILAR", std::bind(&naberezhnov::Commands::hasSimilar, std::ref(inCommands))},
    {L"ISSIMILAR", std::bind(&naberezhnov::Commands::isSimilar, std::ref(inCommands))},
    {L"MERGE", std::bind(&naberezhnov::Commands::merge, std::ref(inCommands))},
    {L"OPEN", std::bind(&naberezhnov::Commands::open, std::ref(inCommands))},
    {L"INSERT", std::bind(&naberezhnov::Commands::insert, std::ref(inCommands))},
    {L"SEARCH", std::bind(&naberezhnov::Commands::search, std::ref(inCommands))},
    {L"DELETEKEY", std::bind(&naberezhnov::Commands::deleteKey, std::ref(inCommands))},
    {L"CHANGETRANSLATE", std::bind(&naberezhnov::Commands::editTranslate, std::ref(inCommands))},
    {L"TRANSLATE", std::bind(&naberezhnov::Commands::translate, std::ref(inCommands))},
    {L"PRINTLETTER", std::bind(&naberezhnov::Commands::printLetter, std::ref(inCommands))},
    {L"PRINT", std::bind(&naberezhnov::Commands::print, std::ref(inCommands))},
    {L"CLOSE", std::bind(&naberezhnov::Commands::close, std::ref(inCommands))}
  };
  std::wstring command = L"";
  std::wcin >> command;
  while (!std::wcin.eof())
  {
    auto iterator = commandsMap.find(command);
    if (iterator != commandsMap.end())
    {
      iterator->second();
    }
    else
    {
      naberezhnov::invalidCommand(std::wcout);
    }
    std::wcin.ignore((std::numeric_limits< std::streamsize >::max)(), '\n');
    std::wcin.clear();
    std::wcin >> command;
  }
  return 0;
}
