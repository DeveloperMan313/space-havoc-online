#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>
#include "Math.h"

class RigidBody {
public:
    struct IntersectionInfo {
        sf::Vector2f median;
        sf::Vector2f normal;
        float depth = 0;
    };

    struct hitboxInfo {
        float radius;
        float width;
        float height;
    };

    enum class rbType {
        circle, rectangle
    };

    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    float elasticity;
    float rotation;
    sf::Vector2f resultantForce = { 0, 0 };
    RigidBody::rbType type;
    RigidBody::hitboxInfo hitbox{};
    sf::Sprite sprite;
    bool deleted = false;
    int id = -1;

    RigidBody();

    RigidBody(enum rbType type, sf::Vector2f position, float rotation, float mass, float elasticity, float radius,
              float width, float height);

    RigidBody(RigidBody const &rb);

    ~RigidBody();

    RigidBody::IntersectionInfo getIntersectionInfo(RigidBody &other) const;

    void applyForce(sf::Vector2f force);

    virtual void physicsStep(float timeDelta);

    virtual void processCollision(RigidBody *other);
};
