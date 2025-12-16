#pragma once

#include "constants.h"
#include "helper.h"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <fmt/base.h>
#include <fstream>
#include <iostream>
#include <spdlog/details/os-inl.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct ParserMemory {
  std::string value{};
  std::string command{};
  std::string destination{};
};

enum class Types { MEMORY, LOGICAL };

class Parser {

private:
  std::string line;
  std::ifstream input_file;
  constexpr static std::array<std::string_view, 9> logical_commands = {
      "add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not"};

public:
  std::vector<std::pair<int, std::string>> commands;
  Parser(const std::string &file_name) {
    input_file.open(file_name);
    if (!input_file.is_open())
      spdlog::error("Error opening file");
  }

  inline void parse_read_file();
  static inline ParserMemory parse_memory_commands(std::string_view line);
  static inline std::string parse_instruction(std::string_view line);

  ~Parser() {
    if (input_file.is_open())
      input_file.close();
  };
};

void Parser::parse_read_file() {

  int type = 0;
  while (std::getline(input_file, line)) {
    std::string ll = parse_instruction(line);
    if (helper_get_first_word(ll) == "push" ||
        helper_get_first_word(ll) == "pop") {
      type = static_cast<int>(Types::MEMORY);
      std::pair<int, std::string> w = {type, ll};
      commands.emplace_back(w);
    } else if (std::find(logical_commands.begin(), logical_commands.end(),
                         ll) != logical_commands.end()) {
      type = static_cast<int>(Types::LOGICAL);
      std::pair<int, std::string> w = {type, ll};
      commands.emplace_back(w);
    }
  };
};

std::string Parser::parse_instruction(std::string_view line) {

  std::string parsed_string(line);
  size_t backslash = line.find("//");

  if (backslash != std::string::npos)
    parsed_string = parsed_string.substr(0, backslash);

  parsed_string.erase(
      parsed_string.begin(),
      std::find_if(parsed_string.begin(), parsed_string.end(),
                   [](unsigned char ch) { return !std::isspace(ch); }));

  parsed_string.erase(
      std::find_if(parsed_string.rbegin(), parsed_string.rend(),
                   [](unsigned char ch) { return !std::isspace(ch); })
          .base(),
      parsed_string.end());

  return parsed_string;
};

ParserMemory Parser::parse_memory_commands(std::string_view line) {
  ParserMemory cmd = {"", "", ""};
  std::array<std::string, 3> words = helper_get_words(line);

  if (words[0] == "push" || words[0] == "pop")
    cmd.command = words[0];

  cmd.destination = words[1];
  cmd.value = words[2];

  return cmd;
};
