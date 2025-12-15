#pragma once

#include "parser.h"
#include "spdlog/spdlog.h"
#include "translate1.h"
#include <iostream>
#include <string>
#include <unordered_map>

class LogicTranslator : public Translator {

public:
  LogicTranslator(const std::string &file_name) : Translator(file_name) {}
  inline void translate_logic_commands(const std::string &command);

private:
  std::unordered_map<std::string, std::string> logic_commands_reference = {
      {"add", "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=D+M\n@SP\nA=M\nM=D\n@"
              "SP\nM=M+1\n"},
      {"sub", "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=D-M\n@SP\nA=M\nM=D\n@"
              "SP\nM=M+1\n"},

  };

  inline std::string translate_operations(const std::string &line);
};

std::string LogicTranslator::translate_operations(const std::string &line) {
  return logic_commands_reference[line];
};

void LogicTranslator::translate_logic_commands(const std::string &command) {

  std::string comment = "// " + command + "\n";
  std::string final_asm = "";
  if (command == "add" || command == "sub") {
    final_asm = translate_operations(command);
  }

  translate_append_file(comment);
  translate_append_file(final_asm);
}
