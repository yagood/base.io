#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include "../network/server.hpp"
#include "../modules/utils.hpp"
#include "../modules/logger.hpp"

struct Commands;

class Game {
  friend struct Commands;
public:
  Game();
  ~ Game();

  void mainLoop();

private:
  void loadConfig();
  void startLogger();

private:
  Server server_;
  bool running = true;
};

namespace cfg {

extern int logger_maxSeverity;
extern int logger_maxFileSeverity;
extern Logger::Color logger_printTextColor;
extern Logger::Color logger_backgroundColor;

extern short server_port;
extern std::string server_name;
extern unsigned long long server_maxConnections;

extern unsigned int game_mode;
extern unsigned int game_timeStep;
extern double game_mapWidth;
extern double game_mapHeight;
extern unsigned int game_quadTreeLeafCapacity;
extern unsigned int game_quadTreeMaxDepth;

extern unsigned int player_viewBoxWidth;
extern unsigned int player_viewBoxHeight;
extern unsigned long long player_cellRemoveTime;
extern unsigned long long player_collisionIgnoreTime;

extern double entity_minEatOverlap;
extern double entity_minEatSizeMult;

extern double cell_baseRadius;
extern double cell_maxRadius;
extern unsigned int cell_speedMultiplier;

extern double food_baseRadius;
extern double food_maxRadius;
extern unsigned int food_startAmount;
extern unsigned int food_maxAmount;

} // namespace config

#endif // GAME_GAME_H_