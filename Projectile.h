#pragma once

#include "RigidBody.h"
#include "Textures.h"

class Projectile : public RigidBody {
public:
    int clientId;

    Projectile(Textures *textures);

    Projectile(const Projectile &rb);

    ~Projectile();

    void processCollision(RigidBody *other);
};
