#ifndef ENTITIES_FOOD_H_
#define ENTITIES_FOOD_H_

#include "entity.hpp"

class Food : public Entity {
public:
  Food(const Vec2& pos, double radius, const Color& color);
  ~Food();

  void onDespawned();
};

#endif // ENTITIES_FOOD_H_