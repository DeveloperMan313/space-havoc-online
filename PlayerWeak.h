#pragma once

#include <iostream>
#include "RigidBody.h"
#include "Textures.h"
#include "Projectile.h"
#include "Laserbeam.h"

class PlayerWeak : public RigidBody {
public:
    int clientId;
    float velocityScalar;
    int rotationDir;
    float spriteHue;
    bool doPUSpawn;
    bool isInvincible;
    sf::Clock LMBelapsed;
    sf::Clock invincibilityClock;
    Textures *textures;
    sf::Clock createdElapsed;

    explicit PlayerWeak(Textures *textures);

    PlayerWeak(const PlayerWeak &rb);

    ~PlayerWeak();

    void physicsStep(float timeDelta);

    void jump();

    void processCollision(RigidBody *other);

    void setHue(float hue);
};
