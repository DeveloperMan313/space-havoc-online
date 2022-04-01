#include "Player.h"

Player::Player(Textures *textures) {
    this->mass = 5;
    this->elasticity = 0.5f;
    this->hitbox.radius = 25;
    this->sprite.setTexture(*textures->player);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->sprite.setScale(sf::Vector2f(5, 5));
    this->sprite.setOrigin(sf::Vector2f(7, 8));
    this->velocityScalar = 200;
    this->rotationDir = 0;
    this->clientId = -1;
    this->ammo = 0;
}

Player::Player(const Player &rb) : RigidBody(rb) {
    this->clientId = rb.clientId;
    this->velocityScalar = rb.velocityScalar;
    this->rotationDir = rb.rotationDir;
    this->ammo = rb.ammo;
    this->LMBelapsed = rb.LMBelapsed;
    this->ammoClock = rb.ammoClock;
}

Player::~Player() = default;

void Player::physicsStep(float timeDelta) {
    this->rotation = this->rotation + 5.4f * (float) this->rotationDir * timeDelta;
    if (this->velocityScalar > 200) this->velocityScalar -= 3000 * timeDelta;
    this->velocity = Math::rotateVector(sf::Vector2f(this->velocityScalar, 0), this->rotation);
    RigidBody::physicsStep(timeDelta);
}

void Player::jump() {
    if (this->velocityScalar <= 200) this->velocityScalar = 1200;
}

void Player::processCollision(RigidBody *other) {
    if (typeid(*other).name() == std::string("class Projectile") &&
        ((Projectile *) other)->clientId != this->clientId) {
        this->deleted = true;
        other->deleted = true;
    }
}
