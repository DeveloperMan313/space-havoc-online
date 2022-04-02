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
    float spriteHue;
    sf::Clock LMBelapsed;
    sf::Clock ammoClock;
    Textures *textures;

    Player(Textures *textures);

    Player(const Player &rb);

    ~Player();

    void physicsStep(float timeDelta);

    void jump();

    void processCollision(RigidBody *other);

    void setHue(float hue);
};
