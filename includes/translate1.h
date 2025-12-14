#pragma once

#include "spdlog/spdlog.h"
#include <fstream>
#include <iostream>
#include <ostream>

class Translator {

  std::ofstream output_file;

protected:
  void translate_append_file(std::string_view line) {
    if (output_file.is_open()) {
      output_file << line;
    } else {
      spdlog::error("Error opening file for appending");
    }
    return;
  };

public:
  Translator(const std::string &file_name)
      : output_file(file_name, std::ios::app) {
    if (!output_file.is_open())
      spdlog::error("Error opening file");
  }
  ~Translator() {
    if (output_file.is_open())
      output_file.close();
  };
};
