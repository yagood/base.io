#ifndef PLAY_PLAYER_H_
#define PLAY_PLAYER_H_

#include "../network/server.hpp"
#include "../network/packet.hpp"
#include "../entities/cell.hpp"

enum struct PlayerState {
  DEAD = 0,
  PLAYING,
  DISCONNECTED
};

class Player {
public:
  Server* server;
  Packet* packet;
  PacketHandler packetHandler;
  uWS::WebSocket<uWS::SERVER>* socket;

  std::shared_ptr<Cell::Entity> cell;

  Player(uWS::WebSocket<uWS::SERVER>* socket);
  ~Player();

  void setDead();

  const std::string& name() { return name_; };
  const Vec2& mouse() { return mouse_; }
  const PlayerState& state() { return state_; }

  void update();

  void onSpawn(std::string name);
  void onTarget(const Vec2& pos);
  void onDisconnection();

private:
  void updateScale();
  void updateCenter();
  void updateViewBox();
  void updateVisibleNodes();

private:
  std::string name_ = "";
  
  float  scale_ = 0.0f;
  double score_ = 0.0;
  float  filteredScale_ = 1.0f;
  
  Vec2 mouse_   = { 0, 0 };
  Vec2 center_  = { 0, 0 };
  Rect viewBox_ = Rect(0, 0, 0, 0);

  std::map<unsigned long long, e_ptr> visibleNodes_;
  PlayerState state_ = PlayerState::DEAD;
};


#endif // PLAY_PLAYER_H_