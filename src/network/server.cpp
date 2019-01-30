#include "../game/game.hpp"
#include "../play/player.hpp"

void Server::start() {
  connectionThread = std::thread([&]() {
    uWS::Hub hub;
    onClientConnection(&hub);
    onClientDisconnection(&hub);
    onClientMessage(&hub);

    if (hub.listen(cfg::server_port)) {
      Logger::warn("Server is listening on port %d\n", cfg::server_port);
      hub.run();
    } else {
      Logger::warn("Server couldn't listen on port %d\n", cfg::server_port);
      end();
    }
  });
}

void Server::onClientConnection(uWS::Hub *hub) {
  hub->onConnection([&](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
    if (++connections >= cfg::server_maxConnections) {
      ws->close(1000, "Server connection limit reached");
        return;
    }
    Player *player = new Player(ws);
    player->server = this;
    ws->setUserData(player);
    clients.push_back(player);

    Logger::debug("Connection made");
  });
}

void Server::onClientDisconnection(uWS::Hub *hub) {
  hub->onDisconnection([&](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
    ((Player*)ws->getUserData())->onDisconnection();
    --connections;
    Logger::debug("Disconnection made");
  });
}

void Server::onClientMessage(uWS::Hub *hub) {
  hub->onMessage([&](uWS::WebSocket<uWS::SERVER>* ws, char* message, size_t length, uWS::OpCode opCode) {
    if (length == 0) return;

    if (length > 256) {
      ws->close(1009, "no spam pls");
      return;
    }
    
    std::vector<unsigned char> packet(message, message + length);
    Player *player = (Player*)ws->getUserData();
    if (player != nullptr)
      player->packetHandler.onPacket(packet);
  });
}

void Server::end() {
  Logger::warn("Stopping uWS Server...");
  for (Player *player : clients)
    player->socket->close();
  
  connectionThread.detach();
}