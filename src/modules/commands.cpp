#include "commands.hpp"
#include "logger.hpp"
#include "../game/game.hpp"
#include "../game/map.hpp"

Commands::Commands(Game *_game) :
  game(_game) {
}

void Commands::handleUserInput(std::string &in) {
  if (in.empty()) return;
  Logger::logMessage(in + '\n'); // Write input to log

  // Remove all spaces from input
  in.erase(std::remove_if(in.begin(), in.end(), [](char c) {
    return c == ' ';
  }), in.end());

  // Get arguments
  std::vector<std::string> args = splitStr(in, '(');
  std::string name = args[0];

  if (in == name || args.back().back() != ')') {
    Logger::warn("Invalid syntax. Try commandName(args)\n");
    return;
  }
  // Extract arguments between parentheses
  unsigned int s = 0;
  for (const std::string &arg : args) s += (unsigned int)arg.size();
  args = splitStr(std::string(in.begin() + name.size() + 1, in.begin() + s), ',');

  // Parse newly extracted arguments
  std::vector<json> parsedArgs;
  for (const std::string &arg : args) {
    try {
      parsedArgs.push_back(json::parse(arg));
    } catch (std::invalid_argument &e) {
      Logger::warn(std::string(e.what()) + "\n");
      return;
    }
  }
  // Parse commands
  if (auto cmd = command[name]) {
    try {
      (this->*cmd)(parsedArgs);
    } catch (const char *e) {
      Logger::warn(e);
    }
  } else {
    Logger::warn("Invalid command. Use 'help()' for a list of commands.");
  }
  Logger::print('\n');
}

void Commands::help(const std::vector<json> &args) {
  if (!args.empty()) throw "help() takes zero arguments.";

  Logger::info("clr() ---------------------- Clears console output.");
  Logger::info("clearMap() ----------------- Clears the map and removes all entities.");
  Logger::info("exit(), -------------------- Stops the game and closes the server.");
}

void Commands::clr(const std::vector<json> &args) {
  if (!args.empty()) throw "exit() takes zero arguments.";
  Logger::clearConsole();
}

void Commands::clearMap(const std::vector<json> &args) {
  if (!args.empty()) throw "clearMap() takes zero arguments.";
  map::clear();
}

void Commands::exit(const std::vector<json> &args) {
  if (!args.empty()) throw "exit() takes zero arguments.";
  Logger::warn("Closing server...");
  game->running = false;
}

void Commands::debug(const std::vector<json> &args) {
  Logger::info("Food: ", map::entities[EntityType::FOOD].size());
  Logger::info("Cells: ", map::entities[EntityType::CELL].size());
  Logger::info("Total quadTree objects: ", map::quadTree.totalObjects());
  Logger::info("Total quadTree children: ", map::quadTree.totalChildren());
}