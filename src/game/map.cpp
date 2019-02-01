#include "map.hpp"
#include "../game/game.hpp"
#include "../modules/logger.hpp"
#include "../entities/cell.hpp"
#include "../entities/food.hpp"

namespace map {

Game* game;
QuadTree quadTree;

std::vector<e_vec> entities{
  std::vector<std::shared_ptr<Cell::Entity>>(),
  std::vector<std::shared_ptr<Food::Entity>>()
};

void init(Game* _game) {
  Logger::info("Initializing Map...");

  game = _game;
  quadTree = QuadTree({ 0, 0, cfg::game_mapWidth, cfg::game_mapHeight }, 
                      cfg::game_quadTreeLeafCapacity, cfg::game_quadTreeMaxDepth);

  // Spawn starting food
  while (entities[EntityType::FOOD].size() < cfg::food_startAmount)
    spawn<Food>(randomPosition(), cfg::food_baseRadius, randomColor());
}

void clear() {
  Logger::warn("Clearing Map...");

  quadTree.clear();
  for (e_vec &entityVec : entities) {
    Logger::logDebug(entityVec.size());
    entityVec.clear();
  }
}

const Rect& bounds() {
    return quadTree.getBounds();
}

template <typename T>
e_ptr spawn(const Vec2 &pos, double radius, const Color &color) {
  e_ptr entity = std::make_shared<T>(pos, radius, color);
  entity->shared = entity;
  const double r = radius * 2;

  entity->obj = Collidable({ pos.x, pos.y, r, r }, entity.get());
  quadTree.insert(&entity->obj);
  entities[entity->type].push_back(entity->shared);
  return entity;
}
template e_ptr spawn<Cell>(const Vec2 &pos, double radius, const Color &color);
template e_ptr spawn<Food>(const Vec2 &pos, double radius, const Color &color);

void despawn(e_ptr &entity) {
  if (!entity) {
    logVerbose(Shared pointer for entity is empty!);
    return;
  }
  if (entity->isRemoved == true) {
    logVerbose(Entity is already removed!);
    return;
  }
  // Erase from vector of its type
  e_vec &vec = entities[entity->type];
  e_vec::iterator index = std::remove(vec.begin(), vec.end(), entity->shared);

  if (index == vec.end()) {
    Logger::warn("not find :", entity->nodeId());
    return;
  }
  entities[entity->type].erase(index);
  // Remove from quadTree
  if (!quadTree.remove(&entity->obj)) {
    logVerbose(Entity could not be removed from quadTree!);
    return;
  }
  entity->isRemoved = true;
  entity->isUpdated = true;
  entity->onDespawned();
}

void update() {
  for (e_ptr &food : entities[EntityType::FOOD]) {
        if (food->isRemoved) continue;
        updateObject(food.get());
    }
  e_vec& pvec = entities[EntityType::CELL];
  for (e_ptr &cell : pvec) {
    if (!cell) {
      logVerbose(Shared pointer for Cell is empty! Removing from vector.);
      e_vec::iterator i = std::remove(pvec.begin(), pvec.end(), cell);
      if (i != pvec.end())
        pvec.erase(i);
      
      continue;
    }
    if (cell->isRemoved) continue;
        
    updateObject(cell.get());
    cell->update();

    for (Collidable *obj : quadTree.getObjectsInBound(cell->obj.bound)) {
      Entity* e = std::any_cast<Entity*>(obj->data);
      handleCollision(cell, e);
    }
    if (!cell) {
      logVerbose(Shared pointer for Cell is empty! Removing from vector.);
      e_vec::iterator i = std::remove(pvec.begin(), pvec.end(), cell);
        if (i != pvec.end())
          pvec.erase(i);
      
      continue;
    }
  }
}
void updateObject(Entity *entity) {
  if (!entity->shared) {
    logVerbose(Shared pointer for entity is empty!!);
    return;
  }
  if (!entity->isUpdated || entity->isRemoved)
    return;

  const double wh = entity->radius() * 2;
  const Vec2 &pos = entity->pos();

  entity->obj.bound.update(pos.x, pos.y, wh, wh);
  if (!quadTree.update(&entity->obj)) {
    logVerbose(Entity could not be updated!);
    despawn(entity->shared);
    //Logger::debug(entity->toString());
    return;
  }
  if (!entity->shared) {
    logVerbose(Shared pointer for entity is empty!!!);
    return;
  }
  entity->isUpdated = false;
}

void handleCollision(e_ptr &cell1, Entity *cell2) {
  if (!cell1 || !cell2->shared)
    return;

  if (cell1->isRemoved || cell2->isRemoved || !cell1->intersects(cell2->shared))
    return;

  if (cell1->radius() <= cell2->radius() * cfg::entity_minEatSizeMult) 
    return;
  
  e_ptr predator = cell1;
  e_ptr prey = cell2->shared;
  predator->consume(prey);
}

} // namespace map

