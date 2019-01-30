#ifndef NETWORK_PACKET_H_
#define NETWORK_PACKET_H_

#include "packet_handler.hpp"
#include "../modules/utils.hpp"

class Packet {
public:
  Packet();

  Buffer& clearAll();
  Buffer& setBorder();
  Buffer& addNode(unsigned int nodeId);
  Buffer& updateNodes(const e_vec& add, const e_vec& update, const e_vec& eat, const e_vec& del);
  Buffer buffer;
};

#endif // NETWORK_PACKET_H_