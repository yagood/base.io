#include "entity.hpp"
#include "../game/game.hpp"
#include "../game/map.hpp"

Entity::Entity(const Vec2& pos, double radius, const Color& color) : 
  pos_(pos),
  color_(color),
  radius_(radius) {
  mass_ = toMass(radius);
  nodeId_ = prevNodeId == 4294967295 ? 1 : ++prevNodeId;
}

Entity::~Entity() {
}

void Entity::setPosition(const Vec2& pos, bool validate) {
  pos_ = pos;
  isUpdated = true;
  
  if (!validate) return; // No need

  //validate
  const double hr = radius_ * 0.5f; // half radius

  // left
  double maxIndent = map::bounds().left() + hr;
  if (pos_.x <= maxIndent) {
    pos_.x = maxIndent;
    vel_.x = -vel_.x;
  }
  // right
  if ((maxIndent = map::bounds().right() - hr) < pos_.x) {
    pos_.x = maxIndent;
    vel_.x = -vel_.x;
  }
  // bottom
  if ((maxIndent = map::bounds().bottom() + hr) > pos_.y) {
    pos_.y = maxIndent;
    vel_.y = -vel_.y;
  }
  // top
  if ((maxIndent = map::bounds().top() - hr) < pos_.y) {
    pos_.y = maxIndent;
    vel_.y = -vel_.y;
  }
}

void Entity::setMass(double mass) {
  mass_ = mass;
  radius_ = toRadius(mass);
  isUpdated = true;
}

bool Entity::intersects(e_ptr other) {
  return intersects(other->pos_, other->radius_);
}

bool Entity::intersects(const Vec2 &pos, double radius) {
  double rs = radius_ + radius;
  return (pos_ - pos).squared() < (rs * rs);
}

void Entity::consume(e_ptr &prey) {
  if (prey->isRemoved)
    return;

  double range = radius_ - cfg::entity_minEatOverlap * prey->radius_;
  if ((pos_ - prey->pos_).squared() >= range * range)
    return; // Not close enough to eat

  prey->killerId_ = nodeId_;
  setMass(mass_ + prey->mass_);
  map::despawn(prey); // remove prey from map
}