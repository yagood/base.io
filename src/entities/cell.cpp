#include "cell.hpp"
#include "../game/game.hpp"
#include "../play/player.hpp"

Cell::Cell(const Vec2 &pos, double radius, const Color &color) :
  Entity(pos, radius, color) {
  type = EntityType::CELL;
}

Cell::~Cell() {
}

void Cell::update() {
  Vec2 diff = (owner_->mouse() - pos_).round();
  double distance = (int)diff.squared();

  // Not enough of a difference to move
  if (distance <= 1) return;

  // distance between mouse and cell
  distance = std::sqrt(distance);

  // https://imgur.com/a/H9s0J
  // s = min(d, 2.2 * (r^-0.4396754) * t * m) / d
  double speed = 2.2 * std::pow(radius_, -0.4396754); // speed per millisecond
  speed *= cfg::game_timeStep * cfg::cell_speedMultiplier; // speed per tick

  // limit the speed and check > 0 to prevent jittering
  speed = std::min(distance, speed) / distance;
  if (speed > 0)
    setPosition(pos_ + diff * speed, true);
}

void Cell::onDespawned() {
  owner_->cell.reset();
  if (owner_->state() != PlayerState::DISCONNECTED)
    owner_->setDead();
}