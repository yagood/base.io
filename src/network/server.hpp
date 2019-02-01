#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_

#include <uWS/Hub.h>
#include <thread>

class Player;

struct Server {
  std::vector<Player*> clients;
  unsigned long long connections = 0;

  void start();
  void onClientConnection(uWS::Hub *hub);
  void onClientDisconnection(uWS::Hub *hub);
  void onClientMessage(uWS::Hub *hub);
  void removeClient(Player* client);
  void end();

private:
  std::thread connectionThread;
};

#endif // NETWORK_SERVER_H_