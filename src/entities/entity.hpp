#ifndef ENTITIES_ENTITY_H_
#define ENTITIES_ENTITY_H_

#include "../modules/utils.hpp"
#include "../modules/quad_tree.hpp"

namespace {
  // Using struct instead of enum for use with vector
  struct EntityType {
    static const int CELL = 0;
    static const int FOOD = 1;
  };
  unsigned int prevNodeId = 0;
}

class Player;

class Entity {
public:
  Entity(const Vec2& pos, double radius, const Color& color);
  virtual ~Entity();

  e_ptr shared;
  Collidable obj;
  
  int type = -1;
  bool isUpdated = true;
  bool isRemoved = false;

  // set
  void setOwner(Player *owner) { owner_ = owner; }
  void setPosition(const Vec2& pos, bool validate = false);
  void setMass(double mass);

  // get
  Player* owner() { return owner_; }
  double mass()   { return mass_ ; }
  double radius() { return radius_; }
  const Vec2& pos() { return pos_; };
  const Color& color() { return color_; }
  uint32_t nodeId() { return nodeId_; }
  uint32_t killerId() { return killerId_; }

  bool intersects(e_ptr other);
  bool intersects(const Vec2 &pos, double radius);
  void consume(e_ptr &prey);

  virtual void update() { }
  virtual void onDespawned() { }
  
protected:
  Vec2 pos_ { 0, 0 };
  Vec2 vel_ { 1, 0 };
  Color color_ { 0, 0, 0 };
  double mass_ = 0;
  double radius_ = 0;

  uint32_t nodeId_ = 0;
  uint32_t killerId_ = 0;

  Player* owner_ = nullptr;
};

#endif // !ENTITIES_ENTITY_H_