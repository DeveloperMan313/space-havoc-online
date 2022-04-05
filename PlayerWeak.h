#pragma once

#include <iostream>
#include "RigidBody.h"
#include "Textures.h"
#include "Projectile.h"

class PlayerWeak : public RigidBody {
public:
    int clientId;
    float velocityScalar;
    int rotationDir;
    sf::Clock LMBelapsed;
    sf::Clock jumpCooldown;
    Textures *textures;

    PlayerWeak(Textures *textures);

    PlayerWeak(const PlayerWeak &rb);

    ~PlayerWeak();

    void physicsStep(float timeDelta);

    void jump();

    void processCollision(RigidBody *other);
};
