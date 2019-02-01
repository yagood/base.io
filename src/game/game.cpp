#include "game.hpp"
#include "map.hpp"
#include "../play/player.hpp"
#include "../modules/commands.hpp"

#include <future>
#include <chrono>
#include <time.h>

using namespace std::chrono;

Game::Game() {
  loadConfig();
  startLogger();
  server_.start();

  map::init(this);
  Commands commands(this);
  std::string userInput;

  nanoseconds fixed_timestep = 16ms;
  nanoseconds lag = 0ns;

  auto startTime = steady_clock::now();

  while (running) {
    std::future<std::string&> future = std::async([&]()->std::string& {
      printf("> ");
      std::getline(std::cin, userInput);
      return userInput;
    });
    while (future.wait_for((milliseconds)40) == std::future_status::timeout) {
      nanoseconds deltaTime = steady_clock::now() - startTime;
      startTime = steady_clock::now();
      
      lag += deltaTime;
      while (lag >= fixed_timestep)
        lag -= fixed_timestep;
      
      mainLoop();
    }
    commands.handleUserInput(future.get());
  }
}

Game::~Game() {
  server_.end(); 
  map::clear();

  Logger::warn("Saving log...");
  Logger::end();
}

void Game::mainLoop() {
  // Update players
  for (Player *client : server_.clients)
    client->update();

  // Update entities
  map::update();
}

void Game::startLogger() {
  Logger::start();
  Logger::PRINT.suffix = "";
  Logger::setSeverity(cfg::logger_maxSeverity);
  Logger::setFileSeverity(cfg::logger_maxFileSeverity);
  Logger::setConsoleColor(cfg::logger_backgroundColor);
  Logger::setTextColor(cfg::logger_printTextColor);
  Logger::ERR.bgColor = cfg::logger_backgroundColor;
  Logger::INFO.bgColor = cfg::logger_backgroundColor;
  Logger::WARN.bgColor = cfg::logger_backgroundColor;
  Logger::PRINT.bgColor = cfg::logger_backgroundColor;
  Logger::FATAL.bgColor = cfg::logger_backgroundColor;
  Logger::DEBUG.bgColor = cfg::logger_backgroundColor;
  Logger::info("Starting logger...");
  Logger::info("Loading configurations...");
}

void Game::loadConfig() {
  cfg::logger_maxSeverity = config["logger"]["maxSeverity"];
  cfg::logger_maxFileSeverity = config["logger"]["maxFileSeverity"];
  cfg::logger_printTextColor = static_cast<Logger::Color>((int)config["logger"]["printTextColor"]);
  cfg::logger_backgroundColor = static_cast<Logger::Color>((int)config["logger"]["backgroundColor"]);

  cfg::server_port = config["server"]["port"];
  cfg::server_name = config["server"]["name"].get<std::string>();
  cfg::server_maxConnections = config["server"]["maxConnections"];
  
  cfg::game_mode = config["game"]["mode"];
  cfg::game_timeStep = config["game"]["timeStep"];
  cfg::game_mapWidth = config["game"]["mapWidth"];
  cfg::game_mapHeight = config["game"]["mapHeight"];
  cfg::game_quadTreeLeafCapacity = config["game"]["quadTreeLeafCapacity"];
  cfg::game_quadTreeMaxDepth = config["game"]["quadTreeMaxDepth"];

  cfg::entity_minEatOverlap = config["entity"]["minEatOverlap"];
  cfg::entity_minEatSizeMult = config["entity"]["minEatSizeMult"];

  cfg::player_viewBoxWidth = config["player"]["viewBoxWidth"];
  cfg::player_viewBoxHeight = config["player"]["viewBoxHeight"];
  cfg::player_collisionIgnoreTime = config["player"]["collisionIgnoreTime"];

  cfg::cell_baseRadius = config["cell"]["baseRadius"];
  cfg::cell_maxRadius = config["cell"]["maxRadius"];
  cfg::cell_speedMultiplier = config["cell"]["speedMultiplier"];

  cfg::food_baseRadius = config["food"]["baseRadius"];
  cfg::food_maxRadius = config["food"]["maxRadius"];
  cfg::food_startAmount = config["food"]["startAmount"];
  cfg::food_maxAmount = config["food"]["maxAmount"];
}

namespace cfg {

int logger_maxSeverity;
int logger_maxFileSeverity;
Logger::Color logger_printTextColor;
Logger::Color logger_backgroundColor;

short server_port;
std::string server_name;
unsigned long long server_maxConnections;

unsigned int game_mode;
unsigned int game_timeStep;
double game_mapWidth;
double game_mapHeight;
unsigned int game_quadTreeLeafCapacity;
unsigned int game_quadTreeMaxDepth;

double entity_minEatOverlap;
double entity_minEatSizeMult;

unsigned int player_viewBoxWidth;
unsigned int player_viewBoxHeight;
unsigned long long player_collisionIgnoreTime;

double cell_baseRadius;
double cell_maxRadius;
unsigned int cell_speedMultiplier;

double food_baseRadius;
double food_maxRadius;
unsigned int food_startAmount;
unsigned int food_maxAmount;

} // namespace config