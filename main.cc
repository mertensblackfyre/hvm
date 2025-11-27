#include "includes/parser.h"
#include "spdlog/spdlog.h"
#include <fmt/base.h>
#include <string>
#include <string_view>

int main(int argc, char **argv) {

  if (argc < 2) {
    spdlog::error("Usage: program <filename> <output filename>");
    return 1;
  }

  std::string input = argv[1];
  std::string output = argv[2];

  Parser parse(input);
  parse.parse_read_file();

  for (auto a : parse.commands) {
    // Access the int
    if (std::holds_alternative<std::string_view>(a)) {
      fmt::print("{} ", std::get<std::string_view>(a));
    }
    if (std::holds_alternative<ParserMemory>(a)) {
      fmt::print("{} ", std::get<ParserMemory>(a).destination);
      fmt::print("{} ", std::get<ParserMemory>(a).command);
      fmt::println("{}", std::get<ParserMemory>(a).value);
    }
  }
  return 0;
}
