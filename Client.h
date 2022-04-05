#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <deque>
#include "RigidBody.h"
#include "Player.h"
#include "PlayerWeak.h"
#include "Projectile.h"
#include "Server.h"
#include "Powerup.h"

class Client {
public:
    struct rbState {
        bool added = false;
        bool deleted = false;
        RigidBody *rb = nullptr;
        int rbId = -1;
    };

    Client(const std::string& ip, int port, std::vector<RigidBody *> *rigidBodies,
           Textures *textures);

    ~Client();

    void pushInput(const Server::clientInput &input);

    void sendInputData();

    void receiveData();

    Client::rbState pullRbState();

    bool hasRbState();

    bool isConnected() const;

    int id() const;

    void disconnect();

private:
    sf::TcpSocket socket;
    std::deque<Server::clientInput> inputQueue;
    std::deque<Client::rbState> rbStateQueue;
    sf::Packet *currentSendPacket;
    sf::Packet *currentReceivePacket;
    std::vector<RigidBody *> *rigidBodies;
    Textures *textures;
    bool clientIsConnected;
    int clientId;
};
