#pragma once

#include "RigidBody.h"
#include "Textures.h"

class Projectile : public RigidBody {
public:
    int clientId;
    Textures* textures;

    Projectile(Textures *textures);

    Projectile(const Projectile &rb);

    ~Projectile();

    void processCollision(RigidBody *other) override;
};
