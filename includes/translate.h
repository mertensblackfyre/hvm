#pragma once

#include "helper.h"
#include "parser.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <array>
#include <fmt/base.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using fmt::print;
using fmt::println;

class Translator {
private:
  std::string fname;
  std::ofstream output_file;
  std::unordered_map<std::string, std::string> memory_commands_reference = {
      {"push", "@SP\nM=M+1\n"},    {"pop", "@SP\nM=M-1\n"},
      {"dest", "@SP\nA=M\nD=M\n"}, {"value", "\nD=A\n"},
      {"local", "@LCL\nM=M+"},     {"argument", "@ARG\nM=M+"},
      {"this", "@THIS\nM=M+"},     {"that", "@THAT\nM=M+"},
      {"static", "M=D"},
  };

public:
  Translator(const std::string &file_name) {
    output_file.open(file_name, std::ios::app);
    if (!output_file.is_open())
      spdlog::error("Error opening file");
  }

  inline void translate_memory_commands(ParserMemory command,
                                        const std::string &fname);
  inline void translate_append_file(std::string_view line);

  ~Translator() {
    if (output_file.is_open())
      output_file.close();
  };
};

void Translator::translate_append_file(std::string_view line) {
  if (output_file.is_open()) {
    output_file << line;
  } else {
    spdlog::error("Error opening file for appending");
  }
  return;
};

void Translator::translate_memory_commands(ParserMemory command,
                                           const std::string &fname) {

  // Add comment
  std::string comment = fmt::format("// {} {} {} \n", command.command,
                                    command.destination, command.value);
  translate_append_file(comment);

  print("{} ", command.command);
  print("{} ", command.destination);
  println("{}", command.value);

  size_t pos = fname.find(".");
  std::string s = fname.substr(0, pos);

  // Handle Value
  std::string value_asm =
      "@" + command.value + memory_commands_reference["value"];
  translate_append_file(value_asm);

  // Handle destination
  if (command.destination == "command") {
    std::string dest_asm = memory_commands_reference["dest"];
    translate_append_file(dest_asm);
  } else if (command.destination == "local" || command.destination == "that" ||
             command.destination == "this" ||
             command.destination == "argument") {
    std::string dest_asm =
        memory_commands_reference[command.destination] + command.value + "\n";
    translate_append_file(dest_asm);
  } else if (command.destination == "static") {
    std::string dest_asm = "@" + s + "." + command.value + "\n" +
                           memory_commands_reference[command.destination] +
                           command.value + "\n";
    translate_append_file(dest_asm);
  };
  // Handle command
  std::string cmd_asm = memory_commands_reference[command.command];
  translate_append_file(cmd_asm);
};
