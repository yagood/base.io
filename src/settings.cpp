#include "modules/utils.hpp"
#include "modules/logger.hpp"

json config = {
  { "logger", {
    { "maxSeverity", 5 },
    { "maxFileSeverity", 5 },
    { "printTextColor", Logger::Color::White },
    { "backgroundColor", Logger::Color::Black },
  }},

  { "server", {
    { "port", 443 },
    { "name", "base.io" },
    { "maxConnections", 500 }
  }},

  { "game", {
    { "mode", 0 },
    { "timeStep", 40 },
    { "mapWidth", 7142 },
    { "mapHeight", 7142 },
    { "quadTreeLeafCapacity", 64 },
    { "quadTreeMaxDepth", 32 }
  }},

  { "player", {
    { "viewBoxWidth", 1920 },
    { "viewBoxHeight", 1080 },
    { "collisionIgnoreTime", 12 }
  }},

  { "entity", {
    { "minEatOverlap", 0.4 },
    { "minEatSizeMult", 1.15 }
  }},

  { "cell", {
    { "baseRadius", 32 },
    { "maxRadius", 1500 },
    { "speedMultiplier", 1 }
  }},

  { "food", {
    { "baseRadius", 10 },
    { "maxRadius", 20 },
    { "startAmount", 1000 },
    { "maxAmount", 6000 }
  }},
};
