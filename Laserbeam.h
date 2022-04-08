#pragma once

#include "RigidBody.h"
#include "Textures.h"
#include "Player.h"
#include "PlayerWeak.h"

class Laserbeam : public RigidBody {
public:
    int clientId;
    Textures *textures;
    sf::Clock createdElapsed;

    explicit Laserbeam(Textures *textures);

    Laserbeam(const Laserbeam &rb);

    ~Laserbeam();

    RigidBody::intersectionInfo getIntersectionInfo(RigidBody &other) const override;

    void processCollision(RigidBody *other) override;

    void physicsStep(float timeDelta) override;
};
