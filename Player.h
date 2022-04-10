#pragma once

#include <iostream>
#include "RigidBody.h"
#include "Textures.h"
#include "Projectile.h"
#include "Powerup.h"
#include "Laserbeam.h"

class Player : public RigidBody {
public:
    int clientId;
    float velocityScalar;
    int rotationDir;
    int ammo;
    float spriteHue;
    sf::Clock LMBelapsed;
    sf::Clock ammoClock;
    sf::Clock invincibilityClock;
    Textures *textures;
    bool hasLaser;
    bool isInvincible;

    explicit Player(Textures *textures);

    Player(const Player &rb);

    ~Player();

    void physicsStep(float timeDelta) override;

    void jump();

    void processCollision(RigidBody *other) override;

    void setHue(float hue);
};
