#pragma once

#include "spdlog/spdlog.h"
#include <fstream>
#include <iostream>
#include <ostream>

class Translator {

protected:
  inline static std::ofstream output_file{};
  void translate_append_file(std::string_view line) {
    if (output_file.is_open()) {
      output_file << line;
      output_file.flush();
    } else {
      spdlog::error("Error opening file for appending");
    }
    return;
  };

public:
  explicit Translator(const std::string &file_name) {

    if (output_file.is_open()) {
      spdlog::info("File already open");
    }
    if (!output_file.is_open()) {
      output_file.open(file_name, std::ios::app);
    };
    if (output_file.fail()) {
      spdlog::error("Error: ios::failbit");
    }
    if (output_file.bad()) {
      spdlog::error("Error: ios::badbit");
    }
  }
  virtual ~Translator() = default;
};
