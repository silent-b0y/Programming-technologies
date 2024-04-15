#ifndef COMMANDS_H
#define COMMANDS_H

#include <vector>
#include <iosfwd>
#include "Dictionary.h"

namespace naberezhnov
{
  struct Commands
  {
  public:
    Commands(std::wistream&, std::wostream&, std::vector< Dictionary >);
    void help();
    void create();
    void list();
    void hasSimilar();
    void isSimilar();
    void merge();
    void open();
    void insert();
    void search();
    void deleteKey();
    void editTranslate();
    void translate();
    void printLetter();
    void print();
    void close();
  private:
    std::wistream& in_;
    std::wostream& out_;
    std::vector< Dictionary > dicts_;
    Dictionary currentDict_;
    bool opened_;
  };
}

#endif
