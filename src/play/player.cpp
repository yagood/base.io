#include "player.hpp"
#include "../game/map.hpp"
#include "../game/game.hpp"

Player::Player(uWS::WebSocket<uWS::SERVER>* _socket) :
  socket(_socket),
  packet(new Packet()),
  packetHandler(PacketHandler(this)) {
}

Player::~Player() {
  Logger::debug(this, " ~Player");
}

void Player::setDead() {
  state_ = PlayerState::DEAD;
  score_ = 0.0;
}

void Player::update() {
  if (state_ == PlayerState::DEAD) {
    updateVisibleNodes();
  }
  else if (state_ == PlayerState::PLAYING) {
    updateScale();
    updateCenter();
    updateViewBox();
    updateVisibleNodes();
  }
  else if (state_ == PlayerState::DISCONNECTED) {
    if (cell)
      map::despawn(cell);
    else
      server->removeClient(this);
  }
}

void Player::updateScale() {
  if (!cell) return;

  score_ = cell->mass();
  scale_ = std::pow((float)std::min(64 / cell->radius(), 1.0), 0.4f);
}

void Player::updateCenter() {
  center_ = cell->pos();
}

void Player::updateViewBox() {
  Vec2 baseResolution(cfg::player_viewBoxWidth, cfg::player_viewBoxHeight);

  filteredScale_ = (9 * filteredScale_ + scale_) / 10;
  Vec2 viewPort = baseResolution / filteredScale_;
  
  viewBox_.update(center_.x, center_.y, viewPort.x, viewPort.y);
}

void Player::updateVisibleNodes() {
  e_vec addNodes, updNodes, eatNodes, delNodes;
  std::map<unsigned long long, e_ptr> newVisibleNodes;

  for (Collidable *obj : map::quadTree.getObjectsInBound(viewBox_)) {
    if (!obj) continue;
    Entity *entity = std::any_cast<Entity*>(obj->data);
    if (!entity) continue;
    if (visibleNodes_.find(entity->nodeId()) == visibleNodes_.end())
      addNodes.push_back(entity->shared);
    if (entity->isUpdated)
      updNodes.push_back(entity->shared);
    newVisibleNodes[entity->nodeId()] = entity->shared;
  }
  for (const auto &[nodeId, entity] : visibleNodes_) {
    if (!entity->isRemoved) continue;
    if (entity->killerId() != 0)
      eatNodes.push_back(entity);
    delNodes.push_back(entity);
  }
  visibleNodes_ = newVisibleNodes;

  if (eatNodes.size() + updNodes.size() + delNodes.size() + addNodes.size() > 0) {
    //Logger::debug("a:", addNodes.size(), "u:", updNodes.size(), "e:", eatNodes.size(), "d:", delNodes.size() );
    packetHandler.sendPacket(packet->updateNodes(addNodes, updNodes, eatNodes, delNodes));
  }
}

void Player::onSpawn(std::string name) {
  name.pop_back();
  name_ = name;

  Vec2   position = randomPosition();
  Color  color    = randomColor();
  double radius   = cfg::cell_baseRadius;

  cell = map::spawn<Cell>(position, radius, color);
  cell->setOwner(this);
  visibleNodes_.clear();
  packetHandler.sendPacket(packet->clearAll());
  packetHandler.sendPacket(packet->addNode(cell->nodeId()));
  Logger::debug("node id:", cell->nodeId());

  state_ = PlayerState::PLAYING;
  mouse_ = cell->pos();
  update();
}

void Player::onTarget(const Vec2& pos) {
  if (state_ == PlayerState::DEAD) return;
  
  if (mouse_ != pos) 
    mouse_ = pos;
  //Logger::debug("mouse:", mouse_.toString());
  updateCenter();
}

void Player::onDisconnection() {
  state_ = PlayerState::DISCONNECTED;
  delete packet;
}
