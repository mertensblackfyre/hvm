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
  MemoryTranslator translator(output);
  LogicTranslator ltranslator(output);
  parse.parse_read_file();

  for (auto ll : parse.commands) {
    fmt::println("{}", ll.first);
    if (ll.first == static_cast<int>(Types::MEMORY)) {
      ParserMemory p = parse.parse_memory_commands(ll.second);
      translator.translate_memory_commands(p, input);
      continue;
    };
    if (ll.first == static_cast<int>(Types::LOGICAL)) {
      ltranslator.translate_logic_commands(ll.second);
      continue;
    };
  };
  return EXIT_SUCCESS;
}
