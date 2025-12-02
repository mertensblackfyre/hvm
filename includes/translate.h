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
      {"push", "@SP\nM=M+1\n"},
      {"pop", "@SP\nM=M-1\n"},
      {"stack", "@SP\nA=M\nD=M\n"},
      {"value", "\nD=A\n"},
      {"local", "@LCL\nM=M+"},
      {"argument", "@ARG\nM=M+"},
      {"this", "@THIS\nM=M+"},
      {"that", "@THAT\nM=M+"},
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

private:
  inline std::string translate_handle_stack(const std::string &command);
  inline std::string translate_handle_value(const std::string &value);
  inline std::string
  translate_handle_destination(const std::string &destination);

  inline std::string translate_handle_locals(const std::string &destination,
                                             const std::string &value);
  inline std::string translate_handle_static(const std::string &fname,
                                             const std::string &value,
                                             const std::string &destination);
};

void Translator::translate_append_file(std::string_view line) {
  if (output_file.is_open()) {
    output_file << line;
  } else {
    spdlog::error("Error opening file for appending");
  }
  return;
};

std::string Translator::translate_handle_stack(const std::string &command) {
  return memory_commands_reference[command];
};

std::string Translator::translate_handle_value(const std::string &value) {
  return "@" + value + memory_commands_reference["value"];
};

std::string
Translator::translate_handle_destination(const std::string &destination) {
  return memory_commands_reference["stack"];
};


/**
 * This functions handles commands like `local`, 'argument', 'this', and 'that'
 *
 * Eg.
 * push segment i (addr=segmentPointer + i, *SP=*addr, SP++)
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
std::string Translator::translate_handle_locals(const std::string &destination,
                                                const std::string &value) {

  std::string dest_asm = memory_commands_reference[destination] + value + "\n";
};
/*
std::string Translator::translate_handle_constant(const std::string &value) {
  std::string dest_asm = memory_commands_reference[""] + value_asm + "\n";
};

std::string
Translator::translate_handle_static(const std::string &fname,
                                    const std::string &value,
                                    const std::string &destination) {

  size_t pos = fname.find(".");
  std::string s = fname.substr(0, pos);
  std::string dest_asm = "@" + s + "." + value + "\n" +
                         memory_commands_reference[destination] + value + "\n";
};
*/
void Translator::translate_memory_commands(ParserMemory command,
                                           const std::string &fname) {

  // Add comment
  std::string comment = fmt::format("// {} {} {} \n", command.command,
                                    command.destination, command.value);
  translate_append_file(comment);

  print("{} ", command.command);
  print("{} ", command.destination);
  println("{}", command.value);

  if (command.destination == "local" || command.destination == "that" ||
      command.destination == "this" || command.destination == "argument") {
    translate_handle_locals(command.destination, command.value);
  } else if (command.destination == "constant") {
  }

  /*
     *
     *
     *  std::string dest_asm =
          memory_commands_reference[command.destination] + command.value + "\n";
      translate_append_file(dest_asm);
     *
     */
};
