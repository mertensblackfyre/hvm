#pragma once

#include "constants.h"
#include "parser.h"
#include "spdlog/spdlog.h"
#include "translate.h"
#include <fmt/base.h>
#include <iostream>
#include <string>
#include <unordered_map>

class LogicTranslator : public Translator {

public:
  LogicTranslator(const std::string &file_name) : Translator(file_name) {}
  inline void translate_logic_commands(const std::string &command);

private:
  inline std::string translate_operations(const std::string &line);
};

std::string LogicTranslator::translate_operations(const std::string &line) {
  return logical_commands.at(line);
};

void LogicTranslator::translate_logic_commands(const std::string &command) {

  std::string comment = "// " + command + "\n";
  std::string final_asm = logical_commands.at(command);


  Translator::translate_append_file(comment);
  Translator::translate_append_file(final_asm);
}
