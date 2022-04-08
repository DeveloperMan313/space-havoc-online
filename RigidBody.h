#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>
#include "Math.h"

class RigidBody {
public:
    struct intersectionInfo {
        sf::Vector2f median = {INFINITY, INFINITY};
        sf::Vector2f normal = {0, 0};
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
    sf::Vector2f resultantForce = {0, 0};
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

    virtual RigidBody::intersectionInfo getIntersectionInfo(RigidBody &other) const;

    void applyForce(sf::Vector2f force);

    virtual void physicsStep(float timeDelta);

    virtual void processCollision(RigidBody *other);
};
