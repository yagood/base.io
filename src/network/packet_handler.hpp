#ifndef NETWORK_PACKET_HANDLER_H_
#define NETWORK_PACKET_HANDLER_H_

#include "../modules/buffer.hpp"
#include "../modules/utils.hpp"

enum struct PacketType : unsigned char {
  SPAWN = 0x0,
  SET_TARGET = 0x10,
  PONG = 0xe3,
  ESTABLISH_CONNECTION = 0xfe,
  CONNECTION_KEY
};

class Player;

class PacketHandler {
public:
  Player* player;
  PacketHandler(Player *owner);
  ~PacketHandler();

  void sendPacket(Buffer&);

  void onPacket(std::vector<unsigned char>& packet);

private:
  void onSpawn(std::string name) const;
  void onTarget(const Vec2& mouse) const;
  void onEstablishedConnection(const unsigned &protocol);
  void onConnectionKey();
};

#endif // NETWORK_PACKET_HANDLER_H_