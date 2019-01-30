#include "food.hpp"
#include "../game/map.hpp"
#include "../game/game.hpp"

Food::Food(const Vec2 &pos, double radius, const Color &color) :
  Entity(pos, radius, color) {
  type = EntityType::FOOD;
}

Food::~Food() {
}

void Food::onDespawned() {
  if (map::entities[EntityType::FOOD].size() < cfg::food_maxAmount)
    map::spawn<Food>(randomPosition(), cfg::food_baseRadius, randomColor());
}