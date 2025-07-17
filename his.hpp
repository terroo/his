#pragma once

#ifdef _WIN32
#include <curses.h>
#include <windows.h>
#else
#include <ncurses.h>
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <locale.h>
#include <unordered_map>
#include <memory>
#include <array>
#include <functional>
#include <filesystem>
#include <optional>

class His {

  std::string home, his_comm, command, input;
  int ch, rows, cols;
  bool m_match, m_icons;

  std::array<std::array<short,3>,8> custom_colors;
  std::vector<int> color_ids;

  std::vector<std::string> load_suggestions(const std::string& filename);
  std::vector<std::string> get_matches(const std::string& input, 
    const std::vector<std::string>& suggestions);
  static const std::function<std::string(const std::string&)> unicode;
  std::string set_prime(const std::string& prime_input);

  public:
    His(const std::optional<bool> match = std::optional<bool>{false},
        const std::optional<bool> icons = std::optional<bool>{false});
    void run();
};
