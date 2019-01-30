#include "packet_handler.hpp"
#include "../modules/logger.hpp"
#include "../play/player.hpp"

PacketHandler::PacketHandler(Player *owner) :
  player(owner) {
}

PacketHandler::~PacketHandler() {
}

void PacketHandler::sendPacket(Buffer &buffer) {
  const std::vector<unsigned char> &buf = buffer.getBuffer();
  if (buf.empty() || !player->socket || player->socket->isClosed())
    return;
  
  player->socket->send(reinterpret_cast<const char *>(buf.data()), buf.size(), uWS::BINARY);
  buffer.clear();
}

void PacketHandler::onPacket(std::vector<unsigned char> &packet) {
  Buffer buffer(packet);

  switch ((PacketType)buffer.readUInt8()) {
  case PacketType::SPAWN:
    onSpawn(buffer.readStr());
    break;
  case PacketType::SET_TARGET:
    onTarget({ (double)buffer.readInt32_LE(), (double)buffer.readInt32_LE() });
    break;
  case PacketType::PONG:
    Logger::info("Pong packet recieved.");
    break;
  case PacketType::ESTABLISH_CONNECTION:
    onEstablishedConnection(buffer.readUInt32_LE());
    break;
  case PacketType::CONNECTION_KEY:
    onConnectionKey();
    break;
  }
}

void PacketHandler::onEstablishedConnection(const unsigned &protocol) {
  Logger::info("Establish Connection packet recieved.");
  Logger::info("Protocol version: " + std::to_string(protocol));
}

void PacketHandler::onConnectionKey() {
  Logger::info("Connection Key packet recieved.");
  sendPacket(player->packet->clearAll());
  sendPacket(player->packet->setBorder());
}

void PacketHandler::onSpawn(std::string name) const {
  Logger::info("Spawn packet recieved.");
  player->onSpawn(name);
}

void PacketHandler::onTarget(const Vec2& mouse) const {
  //Logger::info("Set Target packet recieved.");
  player->onTarget(mouse);
}