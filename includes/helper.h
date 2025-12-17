#pragma once

#include <string>
#include <string_view>
#include <vector>

extern std::string helper_get_first_word(std::string_view line);
extern std::array<std::string, 3> helper_get_words(std::string_view line);
