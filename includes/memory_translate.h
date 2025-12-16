#pragma once

#include "constants.h"
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
  return memory_commands.at(command);
};

std::string MemoryTranslator::translate_handle_value(const std::string &value) {
  return "@" + value + "\n" + memory_commands.at("index");
};

std::string
MemoryTranslator::translate_handle_destination(const std::string &destination) {
  return memory_commands.at("stack");
};

std::string
MemoryTranslator::translate_handle_locals(const std::string &destination,
                                          const std::string &value,
                                          const std::string &command) {
  if (command == "push") {
    std::string asm_index = translate_handle_value(value);
    std::string asm_local = memory_commands.at(destination);
    std::string asm_stack = memory_commands.at("stack");
    std::string asm_op = memory_commands.at("push");
    return asm_index + asm_local + asm_stack + asm_op + "\n";
  } else {
    std::string asm_index = translate_handle_value(value);
    std::string asm_local = memory_commands.at(destination);
    std::string asm_op = memory_commands.at("pop") + memory_commands.at("addr");
    return asm_index + asm_local + asm_op + "\n";
  }
  return "";
};
std::string
MemoryTranslator::translate_handle_constant(const std::string &value) {

  std::string asm_index = translate_handle_value(value);
  std::string asm_stack = memory_commands.at("stack");
  std::string asm_op = memory_commands.at("push");

  return asm_index + asm_stack + asm_op + "\n";
};

std::string
MemoryTranslator::translate_handle_static(const std::string &fname,
                                          const std::string &value,
                                          const std::string &command) {

  size_t pos = fname.find(".");
  std::string s = fname.substr(0, pos);
  std::string asm_op = memory_commands.at(command);
  std::string dest_asm =
      "@" + s + "." + value + "\n" + memory_commands.at("static") + "\n";

  return asm_op + dest_asm;
};

std::string
MemoryTranslator::translate_handle_temp(const std::string &value,
                                        const std::string &command) {

  std::string asm_index = translate_handle_value(value);
  std::string asm_temp = memory_commands.at("temp");

  std::string asm_op = memory_commands.at("pop") + memory_commands.at("addr");

  return asm_index + asm_temp + asm_op;
};

std::string
MemoryTranslator::translate_handle_pointer(const std::string &value,
                                           const std::string &command) {

  if (command == "push" && value == "0") {
    std::string asm_index = memory_commands.at("0_push");
    std::string asm_stack = memory_commands.at("stack");
    std::string asm_op = memory_commands.at("push");
    return asm_index + asm_stack + asm_op;
  }
  if (command == "push" && value == "1") {
    std::string asm_index = memory_commands.at("1_push");
    std::string asm_stack = memory_commands.at("stack");
    std::string asm_op = memory_commands.at("push");
    return asm_index + asm_stack + asm_op;
  }

  if (command == "pop" && value == "0") {
    std::string asm_op = memory_commands.at("pop");
    std::string asm_index = memory_commands.at("0_pop");
    return asm_op + asm_index;
  }

  if (command == "pop" && value == "1") {
    std::string asm_op = memory_commands.at("pop");
    std::string asm_index = memory_commands.at("1_pop");
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
