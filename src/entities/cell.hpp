#ifndef ENTITIES_CELL_H_
#define ENTITIES_CELL_H_

#include "entity.hpp"

class Cell : public Entity {
public:
  Cell(const Vec2& pos, double radius, const Color& color);
  ~Cell();

  void update();
  void onDespawned();
};

#endif // ENTITIES_CELL_H_