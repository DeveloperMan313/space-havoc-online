#pragma once

#include <iostream>
#include "RigidBody.h"
#include "Textures.h"
#include "Projectile.h"

class Player : public RigidBody {
public:
    int clientId;
    float velocityScalar;
    int rotationDir;
    sf::Clock LMBelapsed;

    Player(Textures *textures);

    Player(const Player &rb);

    ~Player();

    void physicsStep(float timeDelta);

    void jump();

    void processCollision(RigidBody *other);
};
