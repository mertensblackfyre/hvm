#pragma once

#include "parser.h"
#include "spdlog/spdlog.h"
#include "translate.h"
#include <iostream>
#include <unordered_map>

class MemoryTranslator : public Translator {

public:
  MemoryTranslator(const std::string &file_name) : Translator(file_name) {}
  inline void translate_memory_commands(ParserMemory command,
                                        const std::string &fname);

private:
  std::unordered_map<std::string, std::string> memory_commands_reference = {
      // pop & push
      {"push", "@SP\nM=M+1\n"},
      {"pop", "@SP\nM=M-1\nA=M\nD=M\n"},
      {"addr", "@R13\nA=M\nM=D\n"},
      {"index", "\nD=A\n"},
      {"stack", "@SP\nA=M\nM=D\n"},

      // Locals
      {"local", "@LCL\nA=D+M\nD=M\n"},
      {"argument", "@ARG\nA=D+M\nD=M\n"},
      {"this", "@THIS\nA=D+M\nD=M\n"},
      {"that", "@THAT\nA=D+M\nD=M\n"},

      // static
      {"static", "M=D\n"},

      // constant
      {"constant", "M=D\n"},

      {"temp", "@R5\nA=M\nM=D+M\n"},

      // pointers
      {"0_push", "@THIS\nA=M\nD=M\n"},
      {"1_push", "@THAT\nA=M\nD=M\n"},
      {"0_pop", "@THIS\nA=D\n"},
      {"1_pop", "@THAT\nA=D\n"},
  };

  inline std::string translate_handle_stack(const std::string &command);
  inline std::string translate_handle_value(const std::string &value);
  inline std::string
  translate_handle_destination(const std::string &destination);

  inline std::string translate_handle_constant(const std::string &value);
  inline std::string translate_handle_locals(const std::string &destination,
                                             const std::string &value,
                                             const std::string &command);
  inline std::string translate_handle_static(const std::string &fname,
                                             const std::string &value,
                                             const std::string &command);

  inline std::string translate_handle_temp(const std::string &value,
                                           const std::string &command);

  inline std::string translate_handle_pointer(const std::string &value,
                                              const std::string &command);
};

std::string
MemoryTranslator::translate_handle_stack(const std::string &command) {
  return memory_commands_reference[command];
};

std::string MemoryTranslator::translate_handle_value(const std::string &value) {
  return "@" + value + memory_commands_reference["index"];
};

std::string
MemoryTranslator::translate_handle_destination(const std::string &destination) {
  return memory_commands_reference["stack"];
};

std::string
MemoryTranslator::translate_handle_locals(const std::string &destination,
                                          const std::string &value,
                                          const std::string &command) {
  if (command == "push") {
    std::string asm_index = translate_handle_value(value);
    std::string asm_local = memory_commands_reference[destination];
    std::string asm_stack = memory_commands_reference["stack"];
    std::string asm_op = memory_commands_reference["push"];
    return asm_index + asm_local + asm_stack + asm_op + "\n";
  } else {
    std::string asm_index = translate_handle_value(value);
    std::string asm_local = memory_commands_reference[destination];
    std::string asm_op =
        memory_commands_reference["pop"] + memory_commands_reference["addr"];
    return asm_index + asm_local + asm_op + "\n";
  }
  return "";
};
std::string
MemoryTranslator::translate_handle_constant(const std::string &value) {

  std::string asm_index = translate_handle_value(value);
  std::string asm_stack = memory_commands_reference["stack"];
  std::string asm_op = memory_commands_reference["push"];

  return asm_index + asm_stack + asm_op + "\n";
};

std::string
MemoryTranslator::translate_handle_static(const std::string &fname,
                                          const std::string &value,
                                          const std::string &command) {

  size_t pos = fname.find(".");
  std::string s = fname.substr(0, pos);
  std::string asm_op = memory_commands_reference[command];
  std::string dest_asm =
      "@" + s + "." + value + "\n" + memory_commands_reference["static"] + "\n";

  return asm_op + dest_asm;
};

std::string
MemoryTranslator::translate_handle_temp(const std::string &value,
                                        const std::string &command) {

  std::string asm_index = translate_handle_value(value);
  std::string asm_temp = memory_commands_reference["temp"];

  std::string asm_op =
      memory_commands_reference["pop"] + memory_commands_reference["addr"];

  return asm_index + asm_temp + asm_op;
};

std::string
MemoryTranslator::translate_handle_pointer(const std::string &value,
                                           const std::string &command) {

  if (command == "push" && value == "0") {
    std::string asm_index = memory_commands_reference["0_push"];
    std::string asm_stack = memory_commands_reference["stack"];
    std::string asm_op = memory_commands_reference["push"];
    return asm_index + asm_stack + asm_op;
  }
  if (command == "push" && value == "1") {
    std::string asm_index = memory_commands_reference["1_push"];
    std::string asm_stack = memory_commands_reference["stack"];
    std::string asm_op = memory_commands_reference["push"];
    return asm_index + asm_stack + asm_op;
  }

  if (command == "pop" && value == "0") {
    std::string asm_op = memory_commands_reference["pop"];
    std::string asm_index = memory_commands_reference["0_pop"];
    return asm_op + asm_index;
  }

  if (command == "pop" && value == "1") {
    std::string asm_op = memory_commands_reference["pop"];
    std::string asm_index = memory_commands_reference["1_pop"];
    return asm_op + asm_index;
  }

  return "";
};
void MemoryTranslator::translate_memory_commands(ParserMemory command,
                                                 const std::string &fname) {

  std::string comment = fmt::format("// {} {} {} \n", command.command,
                                    command.destination, command.value);
  std::string final_asm = "";
  if (command.destination == "local" || command.destination == "that" ||
      command.destination == "this" || command.destination == "argument") {
    final_asm += translate_handle_locals(command.destination, command.value,
                                         command.command);
  } else if (command.destination == "constant") {
    if (command.command == "pop") {
      spdlog::error("Error: constants cant't be popped");
      return;
    }
    final_asm += translate_handle_constant(command.value);
  } else if (command.destination == "static") {
    final_asm += translate_handle_static(fname, command.value, command.command);
  } else if (command.destination == "temp") {
    final_asm += translate_handle_temp(command.value, command.command);
  } else {
    final_asm += translate_handle_pointer(command.value, command.command);
  }

  Translator::translate_append_file(comment);
  Translator::translate_append_file(final_asm);
  return;
};
