#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <deque>
#include "RigidBody.h"
#include "Player.h"
#include "PlayerWeak.h"
#include "Projectile.h"

class Server {
public:
    enum class msgType {
        updateRbs, addRb, deleteRb, clientInput
    };

    struct msg {
        Server::msgType type;
        int rbId;
        RigidBody *rb;
    };

    struct clientInfo {
        sf::TcpSocket *socket;
        int clientId;
    };

    struct clientInput {
        bool isKeyboard = false;
        sf::Event::EventType keyboardEvent;
        sf::Keyboard::Key key;
        bool isMouse = false;
        sf::Event::EventType mouseEvent;
        sf::Mouse::Button mb;
        int clientId;
    };

    struct clientConnection {
        bool connected = false;
        bool disconnected = false;
        int clientId;
    };

    Server(const int port, std::vector<RigidBody *> *rigidBodies);

    ~Server();

    void connectClient();

    void disconnectClient(int i);

    void pushRbMsg(const Server::msg msg);

    Server::clientInput pullInput();

    Server::clientConnection pullConnection();

    void sendRbData();

    void receiveInputData();

    bool hasClientInput();

    bool hasClientConnection();

private:
    sf::TcpListener listener;
    std::vector<clientInfo> clients;
    std::deque<Server::msg> msgQueue;
    std::vector<std::deque<sf::Packet *>> clientPacketQueue;
    std::deque<Server::clientInput> inputQueue;
    std::deque<Server::clientConnection> connectionQueue;
    std::vector<sf::Packet *> currentSendPackets;
    std::vector<sf::Packet *> currentReceivePackets;
    std::vector<RigidBody *> *rigidBodies;
    int nextClientId;
};
