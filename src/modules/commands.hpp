#ifndef MODULES_COMMANDS_H_
#define MODULES_COMMANDS_H_

#include <map>
#include "utils.hpp"

class Game;

struct Commands {
  Commands(Game*);

  void handleUserInput(std::string &in);

  // Commands
  void help(const std::vector<json> &args);
  void exit(const std::vector<json> &args);
  void clr(const std::vector<json> &args);
  void clearMap(const std::vector<json> &args);
  void debug(const std::vector<json> &args);

private:
  std::map<std::string, void(Commands::*)(const std::vector<json>&)> command = {
    { "help", &Commands::help },
    { "exit", &Commands::exit },
    { "clr", &Commands::clr },
    { "clearMap", &Commands::clearMap },
    { "debug", &Commands::debug }
  };
  Game *game = nullptr;
};

#endif // MODULES_COMMANDS_H_
