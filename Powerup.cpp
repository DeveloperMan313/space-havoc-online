#include "Powerup.h"

Powerup::Powerup(Textures *textures) {
    this->mass = 5;
    this->elasticity = 1.f;
    this->hitbox.radius = 16;
    this->textures = textures;
    this->consumedByClientId = -1;
}

Powerup::Powerup(const Powerup &rb) {
    this->mass = 5;
    this->elasticity = 1.f;
    this->hitbox.radius = 16;
    this->textures = rb.textures;
    this->consumedByClientId = -1;
    this->type = rb.type;
}

Powerup::~Powerup() = default;

void Powerup::processCollision(RigidBody *other) {
    if (this->deleted) return;
    if (typeid(*other).name() == std::string("class Player")) {
        this->consumedByClientId = ((Player *) other)->clientId;
        this->deleted = true;
    }
}

void Powerup::loadSprite() {
    switch (this->type) {
        case Powerup::powerupType::reverse:
            this->sprite.setTexture(*this->textures->powerupReverse);
            break;
        default:
            break;
    }
    this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->sprite.setScale(sf::Vector2f(2, 2));
    this->sprite.setOrigin(sf::Vector2f(8, 8));
}
