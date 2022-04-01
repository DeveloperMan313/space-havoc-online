#include "Projectile.h"

Projectile::Projectile(Textures *textures) {
    this->mass = 1;
    this->elasticity = 1.f;
    this->hitbox.radius = 6;
    this->sprite.setTexture(*textures->projectile);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
    this->sprite.setScale(sf::Vector2f(4, 4));
    this->sprite.setOrigin(sf::Vector2f(1, 1.5));
    this->clientId = -1;
}

Projectile::~Projectile() = default;

void Projectile::processCollision(RigidBody *other) {
    this->deleted = true;
    if (typeid(*other).name() == std::string("class Projectile")) other->deleted = true;
}

Projectile::Projectile(const Projectile &rb) : RigidBody(rb) {
    this->clientId = rb.clientId;
}
