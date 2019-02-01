#include "packet.hpp"
#include "../game/map.hpp"
#include "../play/player.hpp"
#include "../modules/logger.hpp"

Packet::Packet() {
}

Buffer &Packet::clearAll() {
  return buffer.writeUInt8(0x14);
}

Buffer &Packet::addNode(unsigned int nodeId) {
  buffer.writeUInt8(0x20);
  return buffer.writeUInt32_LE(nodeId);
}

Buffer &Packet::updateNodes(const e_vec& add, const e_vec& update, const e_vec& eat, const e_vec& del) {
  buffer.writeUInt8(0x10);
  // eat
  buffer.writeUInt16_LE((unsigned short)eat.size());
  for (const e_ptr &entity : eat) {
    buffer.writeUInt32_LE(entity->killerId());
    buffer.writeUInt32_LE((unsigned)entity->nodeId());
  }
  // add
  for (const e_ptr &entity : add) {
    buffer.writeUInt32_LE((unsigned)entity->nodeId());
    buffer.writeInt32_LE((int)entity->pos().x);
    buffer.writeInt32_LE((int)entity->pos().y);
    buffer.writeUInt16_LE((unsigned short)entity->radius());
    buffer.writeUInt8(entity->color().r);
    buffer.writeUInt8(entity->color().g);
    buffer.writeUInt8(entity->color().b);
    buffer.writeUInt8(0); // flag
    if (entity->type == EntityType::CELL) {
      //Logger::debug(entity->owner()->name());
      buffer.writeStrNull(entity->owner()->name());
    }  
    buffer.writeUInt16_LE(0);
  }
  // update
  for (const e_ptr &entity : update) {
    buffer.writeUInt32_LE((unsigned)entity->nodeId());
    buffer.writeInt32_LE((int)entity->pos().x);
    buffer.writeInt32_LE((int)entity->pos().y);
    buffer.writeUInt16_LE((unsigned short)entity->radius());
    buffer.writeUInt8(entity->color().r);
    buffer.writeUInt8(entity->color().g);
    buffer.writeUInt8(entity->color().b);
    buffer.writeUInt8(0); // flag
    buffer.writeUInt16_LE(0); // empty name

    // if (entity->radius() > 10) {
    //   Logger::debug("size:", (unsigned short)entity->radius());
    // }
  }
  buffer.writeUInt32_LE(0); // update end
  // del
  buffer.writeUInt32_LE((unsigned)del.size());
  for (const e_ptr &entity : del)
    buffer.writeUInt32_LE((unsigned)entity->nodeId());

  return buffer;
}

Buffer &Packet::setBorder() {
  buffer.writeUInt8(0x40);

  buffer.writeDouble_LE(map::bounds().left());
  buffer.writeDouble_LE(map::bounds().bottom());
  buffer.writeDouble_LE(map::bounds().right());
  return buffer.writeDouble_LE(map::bounds().top());
}