#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#include "../entities/entity.hpp"

class Game;

namespace map {
void init(Game* _game);
void clear();

const Rect& bounds();

template <typename T>
e_ptr spawn(const Vec2 &pos, double radius, const Color &color);
void despawn(e_ptr &entity);

void update();
void updateObject(Entity *entity);
void handleCollision(e_ptr &cell1, Entity *cell2);
void resolveCollision(e_ptr cell1, e_ptr cell2);

extern Game* game;
extern QuadTree quadTree;
extern std::vector<e_vec> entities;
}

#endif // GAME_MAP_H_