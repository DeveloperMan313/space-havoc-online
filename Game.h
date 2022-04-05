#pragma once
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include "Math.h"
#include "RigidBody.h"
#include "Textures.h"
#include "Player.h"
#include "PlayerWeak.h"
#include "Projectile.h"
#include "Server.h"
#include "Client.h"
#include "Powerup.h"

class Game {
public:
    Game(bool isServer, const std::string &ip, int port);

    ~Game();

    void initField();

    void update();

    void render();

    bool isRunning();

    void addRigidBody(RigidBody *rigidBody);

    void deleteRigidBody(int id);

    void physicsUpdate(float timeDelta);

    void processClientInput(Server::clientInput input);

private:
    sf::RenderWindow *window;
    Textures *textures;
    std::vector<RigidBody *> rigidBodies;
    int rotationDir;
    bool isServer;
    Server *server;
    Client *client;
    sf::Clock sendRbUpdateClock;
    int nextRbId;
};
