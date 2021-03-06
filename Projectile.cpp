#include "Projectile.h"

Projectile::Projectile(Textures *textures) : RigidBody() {
    this->type = RigidBody::rbType::circle;
    this->mass = 1;
    this->elasticity = 1.f;
    this->hitbox.radius = 6;
    this->textures = textures;
    this->sprite.setTexture(*textures->projectile);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
    this->sprite.setScale(sf::Vector2f(4, 4));
    this->sprite.setOrigin(sf::Vector2f(1, 1.5));
    this->clientId = -1;
}

Projectile::~Projectile() = default;

void Projectile::processCollision(RigidBody *other) {
    this->deleted = true;
}

Projectile::Projectile(const Projectile &rb) : RigidBody(rb) {
    this->type = RigidBody::rbType::circle;
    this->mass = 1;
    this->elasticity = 1.f;
    this->hitbox.radius = 6;
    this->textures = rb.textures;
    this->sprite.setTexture(*this->textures->projectile);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
    this->sprite.setScale(sf::Vector2f(4, 4));
    this->sprite.setOrigin(sf::Vector2f(1, 1.5));
    this->clientId = rb.clientId;
}
