#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <deque>
#include "RigidBody.h"
#include "Player.h"
#include "PlayerWeak.h"
#include "Projectile.h"
#include "Powerup.h"
#include "Laserbeam.h"

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
        sf::Event::EventType keyboardEvent = sf::Event::KeyPressed;
        sf::Keyboard::Key key = sf::Keyboard::Key::Escape;
        bool isMouse = false;
        sf::Event::EventType mouseEvent = sf::Event::MouseButtonPressed;
        sf::Mouse::Button mb = sf::Mouse::Button::Left;
        int clientId = -1;
    };

    struct clientConnection {
        bool connected = false;
        bool disconnected = false;
        int clientId = -1;
    };

    Server(int port, std::vector<RigidBody *> *rigidBodies);

    ~Server();

    void connectClient();

    void disconnectClient(int i);

    void pushMsg(msg msg);

    Server::clientInput pullInput();

    Server::clientConnection pullConnection();

    void sendData();

    void receiveInputData();

    bool hasClientInput();

    bool hasClientConnection();

    static void appendRbToPacket(sf::Packet &packet, RigidBody *rb);

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
