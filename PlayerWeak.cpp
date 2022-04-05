#include "PlayerWeak.h"

PlayerWeak::PlayerWeak(Textures *textures) {
    this->type = RigidBody::rbType::circle;
    this->mass = 2;
    this->elasticity = 0.5f;
    this->hitbox.radius = 16;
    this->sprite.setTexture(*textures->playerWeak);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8));
    this->sprite.setScale(sf::Vector2f(5, 5));
    this->sprite.setOrigin(sf::Vector2f(4, 4));
    this->velocityScalar = 200;
    this->rotationDir = 0;
    this->clientId = -1;
    this->textures = textures;
}

PlayerWeak::PlayerWeak(const PlayerWeak &rb) {
    this->type = RigidBody::rbType::circle;
    this->mass = 2;
    this->elasticity = 0.5f;
    this->hitbox.radius = 16;
    this->textures = rb.textures;
    this->sprite.setTexture(*this->textures->playerWeak);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 8, 8));
    this->sprite.setScale(sf::Vector2f(5, 5));
    this->sprite.setOrigin(sf::Vector2f(4, 4));
    this->clientId = rb.clientId;
    this->velocityScalar = rb.velocityScalar;
    this->rotationDir = rb.rotationDir;
    this->LMBelapsed = rb.LMBelapsed;
    this->jumpCooldown = rb.jumpCooldown;
}

PlayerWeak::~PlayerWeak() = default;

void PlayerWeak::physicsStep(float timeDelta) {
    this->rotation = this->rotation + 0.02f * (float) this->rotationDir;
    if (this->velocityScalar > 200) this->velocityScalar -= 10;
    this->velocity = Math::rotateVector(sf::Vector2f(this->velocityScalar, 0), this->rotation);
    RigidBody::physicsStep(timeDelta);
}

void PlayerWeak::jump() {
    this->velocityScalar = 1000;
}

void PlayerWeak::processCollision(RigidBody *other) {
    if (typeid(*other).name() == std::string("class Projectile") &&
        ((Projectile *) other)->clientId != this->clientId) {
        this->deleted = true;
    }
}
