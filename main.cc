#include "includes/parser.h"
#include "includes/translate.h"
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
  Translator translator(output);
  parse.parse_read_file();

  for (auto ll : parse.commands) {
    if (ll.first == MEMORY) {
      ParserMemory p = parse.parse_memory_commands(ll.second);
      translator.translate_memory_commands(p, input);
    };
  };
  return EXIT_SUCCESS;
}
