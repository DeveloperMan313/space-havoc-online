#pragma once

#include <cstring>
#include "RigidBody.h"
#include "Textures.h"
#include "Player.h"

class Powerup : public RigidBody {
public:
    enum class powerupType { reverse };

    Powerup::powerupType type;
    Textures* textures;
    int consumedByClientId;

    Powerup(Textures *textures);

    Powerup(const Powerup &rb);

    ~Powerup();

    void processCollision(RigidBody *other) override;

    void loadSprite();
};
