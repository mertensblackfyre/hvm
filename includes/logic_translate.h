#pragma once

#include "parser.h"
#include "spdlog/spdlog.h"
#include "translate1.h"
#include <iostream>
#include <unordered_map>

class LogicTranslator : public Translator {

public:
  LogicTranslator(const std::string &file_name) : Translator(file_name) {}
  inline void translate_logic_commands(ParserMemory command,
                                       const std::string &fname);

private:
  std::unordered_map<std::string, std::string> logic_commands_reference = {
      "add", ""};

};
