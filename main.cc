#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {

  if (argc < 3) {
    std::cerr << "Usage: program <filename> <output filename>\n";
    return 1;
  }

  std::string input = argv[1];
  std::string output = argv[2];

  return 0;
}
