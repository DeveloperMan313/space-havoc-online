#include "Powerup.h"

Powerup::Powerup(Textures *textures) : RigidBody() {
    RigidBody::type = RigidBody::rbType::circle;
    this->type = Powerup::powerupType::reverse;
    this->mass = 5;
    this->elasticity = 1.f;
    this->hitbox.radius = 16;
    this->textures = textures;
}

Powerup::Powerup(const Powerup &rb) : RigidBody(rb) {
    RigidBody::type = RigidBody::rbType::circle;
    this->mass = 5;
    this->elasticity = 1.f;
    this->hitbox.radius = 16;
    this->textures = rb.textures;
    this->type = rb.type;
}

Powerup::~Powerup() = default;

void Powerup::processCollision(RigidBody *other) {

}

void Powerup::loadSprite() {
    switch (this->type) {
        case Powerup::powerupType::reverse:
            this->sprite.setTexture(*this->textures->powerupReverse);
            break;
        case Powerup::powerupType::laser:
            this->sprite.setTexture(*this->textures->powerupLaser);
            break;
        default:
            break;
    }
    this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->sprite.setScale(sf::Vector2f(2, 2));
    this->sprite.setOrigin(sf::Vector2f(8, 8));
}
