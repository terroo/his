#include "his.hpp"

const auto help = [](){
  std::printf("Usage:\n"
    "his [options]\n\n"
    "Options:\n"
    "  --match-start,   -m  Match only the exact command.\n"
    "  --no-show-icons, -n  No displays icons.\n"
    "  --help,          -h  Show this message.\n"
    "  --version,       -v  Show version info.\n");
};

int main(int argc, char** argv){
  setlocale(LC_ALL, "");
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  bool match = {false}, icons = {true};
  if(argc > 1){

    for(int i = 1; i < argc; ++i){
      const std::string arg = argv[i];
      if(arg == "--match-start" || arg == "-m"){
        match = true;
      }else if(arg == "--no-show-icons" || arg == "-n"){
        icons = false;
      }else if(arg == "--help" || arg == "-h"){
        help();
        return EXIT_SUCCESS;
      }else if(arg == "--version" || arg == "-v"){
        std::printf("his v0.0.1\n");
        return EXIT_SUCCESS;
      }else{
        std::printf("Invalid argument. Use: %s --help\n", argv[0]);
        return EXIT_FAILURE;
      }
    }
  }
   
  auto his = std::make_unique<His>(match, icons);
  his->run();
}
