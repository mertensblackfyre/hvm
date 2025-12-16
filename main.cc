#include "includes/logic_translate.h"
#include "includes/memory_translate.h"
#include "includes/parser.h"
#include "spdlog/spdlog.h"
#include <cstdlib>
#include <fmt/base.h>
#include <string>

int main(int argc, char **argv) {

  if (argc < 3) {
    spdlog::error("Usage: program <filename> <output filename>");
    return 1;
  }

  std::string input = argv[1];
  std::string output = argv[2];

  Parser parse(input);
  MemoryTranslator memory_translate(output);
  LogicTranslator logic_translate(output);
  parse.parse_read_file();

  for (auto ll : parse.commands) {
    ParserMemory p;
    switch (ll.first) {
    case static_cast<int>(Types::MEMORY):
      p = parse.parse_memory_commands(ll.second);
      memory_translate.translate_memory_commands(p, input);
      break;
    case static_cast<int>(Types::LOGICAL):
      logic_translate.translate_logic_commands(ll.second);
      break;
    default:
      break;
    }
  };
  return EXIT_SUCCESS;
}
