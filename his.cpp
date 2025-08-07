#include "his.hpp"
#include <cstring>

auto rgb_to_ncurses = [](int r, int g, int b) {
  return std::tuple{ r * 1000 / 255, g * 1000 / 255, b * 1000 / 255 };
};

static std::string get_history_file(const std::string& home, const char* env_var, const std::string& default_file) {
  const char* histfile = std::getenv(env_var);
  if (histfile && std::strlen(histfile) > 0) {
    std::filesystem::path hist_path(histfile);
    if (hist_path.is_absolute()) {
      return histfile;
    } else {
      return home + "/" + std::string(histfile);
    }
  }
  return home + "/" + default_file;
}

His::His(const std::optional<bool> match,
    const std::optional<bool> icons) 
  : m_match(match.value()), m_icons(icons.value()) {

#ifdef _WIN32
  home = std::getenv("APPDATA");
  his_comm = home + "\\Microsoft\\Windows\\PowerShell\\PSReadLine\\ConsoleHost_history.txt";
#else
  home = std::getenv("HOME");
  const auto shell = std::getenv("SHELL");
  try {
    if (!shell) {
      throw std::runtime_error("SHELL environment variable is not set. Cannot determine current shell.");
    }else if(std::filesystem::path(shell).filename() == "bash"){
      his_comm = get_history_file(home, "HISTFILE", ".bash_history");
    }else if(std::filesystem::path(shell).filename() == "zsh"){
      his_comm = get_history_file(home, "HISTFILE", ".zsh_history");
    }else{
      throw std::runtime_error("Your default SHELL is not supported.");
    }
  } catch (const std::exception& e) {
    std::printf("his: %s", e.what());
  }

#endif

  initscr();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  use_default_colors();
  init_pair(99, COLOR_YELLOW, -1);

  if(!can_change_color() || COLORS < 16){
    endwin();
    throw std::runtime_error("Your terminal does not support color reset.");
  }

  custom_colors = {{
    {249,38,114},  // PINK
    {166,226,46},  // GREEN
    {230,219,116}, // YELLOW
    {102,217,239}, // BLUE_LIGHT
    {253,151,31},  // ORANGE
    {190,132,255}, // PURPLE
    {230,45,100},  // PINK_FOUR
    {68,68,68}     // GRAY_FOUR
  }};

  for(std::size_t i = 0; i < 8; ++i){
    short r, g, b;
    std::tie(r, g, b) = rgb_to_ncurses(
        custom_colors[i][0],
        custom_colors[i][1],
        custom_colors[i][2]
        );
    short color_index = static_cast<short>(16 + i);
    init_color(color_index, r, g, b);
    init_pair(static_cast<short>(i + 1), color_index, -1);
    color_ids.push_back(static_cast<short>(i + 1));
  }

  command = input = {};
  ch = rows = cols = {};
}

void His::run(){

  auto suggestions = load_suggestions(his_comm);

  while(true){
    getmaxyx(stdscr, rows, cols);

    clear();

    auto matches = get_matches(input, suggestions);
    if(!matches.empty()){
      command = matches[0];
    }else{
      command.clear();
    }

    for (size_t i = 0; i < matches.size() && i < (size_t)(rows - 1); ++i) {
      std::array<char, 20> num;
      snprintf(num.data(), num.size(), "%02lu", i + 1);


      attron(COLOR_PAIR(8) | A_BOLD);
      mvprintw(static_cast<int>(i), 0, "%s", num.data());
      attroff(COLOR_PAIR(8) | A_BOLD);

      std::string prime = set_prime(matches[i]);

      int pair_id = color_ids[i % color_ids.size()] - 1;
      attron(COLOR_PAIR(pair_id));
      if(m_icons){
        mvprintw(static_cast<int>(i), 3, " %s %s", unicode(prime).c_str(), matches[i].c_str());
      }else{
        mvprintw(static_cast<int>(i), 3, " %s", matches[i].c_str());
      }
      attroff(COLOR_PAIR(pair_id));
    }

    attron(A_BOLD | COLOR_PAIR(99));
    mvprintw(rows - 1, 0, "~> %s", input.c_str());
    attroff(A_BOLD | COLOR_PAIR(99));

    move(rows - 1, static_cast<int>(3 + input.size() ) );

    refresh();

    ch = getch();
    if(ch == '\n' || ch == KEY_ENTER){
      delwin(stdscr);
      endwin();
      if(!input.empty()){
        int ret = std::system(command.c_str());
        if(ret != 0){
          delwin(stdscr);
          endwin();
          try{
            throw std::runtime_error("Failed to execute command.");
          }catch(const std::exception& e){
            std::printf("his: %s\n", e.what());
          }
        }
      }
      break;
    }

    if(ch == KEY_BACKSPACE || ch == 127 || ch == 8){
      if (!input.empty()) input.pop_back();
    }else if (isprint(ch)){
      input += (char)ch;
    }
  }

  delwin(stdscr);
  endwin();
}

const std::function<std::string(const std::string&)> His::unicode = [](const std::string& str) {
  static const std::unordered_map<std::string, std::string> html = {
    {"ter", "\uf68c"},
    {"g++", "\ufb71"},
    {"clang++", "\ufb71"},
    {"clang", "\ufb70"},
    {"gcc", "\ufb70"},
    {"python", "\uf81f"},
    {"vim", "\ue62b"},
    {"nvim", "\ue62b"},
    {"cmake", "\uf425"},
    {"git", "\ue5fb"},
    {"sh", "\uf68c"},
    {"bash", "\uf68c"},
    {"zsh", "\uf68c"},
    {"dir", "\uf74a"},
    {"ls", "\uf74a"},
    {"apt", "\uf306"},
    {"java", "\uf675"},
    {"javac", "\uf675"},
    {"jshell", "\uf675"},
    {"php", "\ue73d"},
    {"lua", "\ue620"},
    {"node", "\ue74e"},
    {"npm", "\ue74e"},
    {"bun", "\ue74e"},
    {"qjs", "\ue74e"},
    {"swift", "\ufbe3"},
    {"zip", "\uf410"},
    {"tar", "\uf410"},
    {"make", "\uf728"},
    {"mpg123", "\ufb75"},
    {"ffmpeg", "\ue271"},
    {"ffplay", "\ue271"},
    {"go",  "\ufcd1"},
    {"rustc", "\ue7a8"},
    {"cargo", "\ue7a8"},
    {"ruby", "\ue739"},
    {"mysql", "\ue704"},
    {"psql", "\ue704"},
    {"sqlite3", "\ue7c4"},
    {"sqlite", "\ue7c4"},
  };
  auto it = html.find(str);
  return it != html.end() ? it->second : html.at("ter");
};

std::vector<std::string> His::load_suggestions(const std::string& filename){
  std::vector<std::string> result = {};
  std::ifstream file(filename);
  std::string line = {};
  while(std::getline(file, line)){
    if(!line.empty()) result.push_back(line);
  }
  return result;
}

std::vector<std::string> His::get_matches(const std::string& l_input, 
    const std::vector<std::string>& l_suggestions){
  std::vector<std::string> matches;
  for(const auto& word : l_suggestions){
    if(!m_match){
      if(word.find(l_input) != std::string::npos) matches.push_back(word);
    }else{
      if(word.find(input) == 0) matches.push_back(word);
    }
  }
  return matches;
}


std::string His::set_prime(const std::string& prime_input){
    std::istringstream iss(prime_input);
    std::string word;
    iss >> word;
    if(word == "sudo") iss >> word;
    return word;
}
