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
    int ammo;
    sf::Clock LMBelapsed;
    sf::Clock ammoClock;

    Player(Textures *textures);

    Player(const Player &rb);

    ~Player();

    void physicsStep(float timeDelta);

    void jump();

    void processCollision(RigidBody *other);
};
