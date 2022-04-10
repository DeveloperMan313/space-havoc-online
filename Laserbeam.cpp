#include "Laserbeam.h"

Laserbeam::Laserbeam(Textures *textures) : RigidBody() {
    this->hitbox = {0, 0, 0};
    this->mass = 0;
    this->clientId = -1;
    this->textures = textures;
    this->sprite.setTexture(*textures->laserbeam);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 16 * 100, 16));
    this->sprite.setScale(sf::Vector2f(3, 3));
    this->sprite.setOrigin(sf::Vector2f(0, 8));
}

Laserbeam::Laserbeam(const Laserbeam &rb) : RigidBody(rb) {
    this->hitbox = {0, 0, 0};
    this->mass = 0;
    this->position = rb.position;
    this->rotation = rb.rotation;
    this->clientId = rb.clientId;
    this->createdElapsed = rb.createdElapsed;
    this->textures = rb.textures;
    this->sprite = rb.sprite;
}

Laserbeam::~Laserbeam() = default;

RigidBody::intersectionInfo Laserbeam::getIntersectionInfo(RigidBody &other) const {
    RigidBody::intersectionInfo info{};
    if (other.type == RigidBody::rbType::rectangle) return info;
    Math::line line = Math::linePointsToEquation(this->position,
                                                 this->position +
                                                 Math::rotateVector(sf::Vector2f(1, 0), this->rotation));
    if (Math::distanceToPoint(line, other.position) < other.hitbox.radius) info.median = {0, 0};
    return info;
}

void Laserbeam::processCollision(RigidBody *other) {

}

void Laserbeam::physicsStep(float timeDelta) {
    this->sprite.setPosition(this->position);
    this->sprite.setRotation(this->rotation * 180.f / 3.1415f);
}
