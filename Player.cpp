#include "Player.h"

Player::Player(Textures *textures) : RigidBody() {
    this->type = RigidBody::rbType::circle;
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
    this->spriteHue = 0;
    this->textures = textures;
    this->hasLaser = false;
}

Player::Player(const Player &rb) : RigidBody(rb) {
    this->type = RigidBody::rbType::circle;
    this->mass = 5;
    this->elasticity = 0.5f;
    this->hitbox.radius = 25;
    this->textures = rb.textures;
    this->sprite.setTexture(*this->textures->player);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->sprite.setScale(sf::Vector2f(5, 5));
    this->sprite.setOrigin(sf::Vector2f(7, 8));
    this->velocityScalar = rb.velocityScalar;
    this->rotationDir = rb.rotationDir;
    this->clientId = rb.clientId;
    this->ammo = rb.ammo;
    this->spriteHue = rb.spriteHue;
    this->LMBelapsed = rb.LMBelapsed;
    this->ammoClock = rb.ammoClock;
    this->hasLaser = rb.hasLaser;
}

Player::~Player() {
    this->textures->customPlayerTextures.erase(this->clientId);
}

void Player::physicsStep(float timeDelta) {
    this->rotation = this->rotation + 5.4f * (float) this->rotationDir * timeDelta;
    if (this->velocityScalar > 200) this->velocityScalar -= 3000 * timeDelta;
    this->velocity = Math::rotateVector(sf::Vector2f(this->velocityScalar, 0), this->rotation);
    RigidBody::physicsStep(timeDelta);
}

void Player::jump() {
    if (this->velocityScalar <= 200) {
        this->velocityScalar = 1200;
        if (this->ammo > 0) {
            this->ammo--;
            this->ammoClock.restart();
        }
    }
}

void Player::processCollision(RigidBody *other) {
    if (typeid(*other).name() == std::string("class Projectile") &&
        ((Projectile *) other)->clientId != this->clientId) {
        this->deleted = true;
        other->deleted = true;
    } else if (typeid(*other).name() == std::string("class Powerup") && !other->deleted) {
        switch (((Powerup *) other)->type) {
            case Powerup::powerupType::laser:
                this->hasLaser = true;
                break;
            default:
                break;
        }
        other->deleted = true;
    }
}

void Player::setHue(float hue) {
    this->textures->customPlayerTextures.erase(this->clientId);
    sf::Image image = this->textures->player->copyToImage();
    for (int y = 0; y < image.getSize().y; y++) {
        for (int x = 0; x < image.getSize().x; x++) {
            std::tuple<float, float, float> hsl = Math::RGBtoHSL(image.getPixel(x, y));
            std::get<0>(hsl) = (float) hue;
            std::get<1>(hsl) = 0.7f;
            sf::Color newColor = Math::HSLtoRGB(std::get<0>(hsl), std::get<1>(hsl), std::get<2>(hsl));
            newColor.a = image.getPixel(x, y).a;
            image.setPixel(x, y, newColor);
        }
    }
    auto *newTexture = new sf::Texture;
    newTexture->loadFromImage(image);
    this->sprite.setTexture(*newTexture);
    this->textures->customPlayerTextures.insert({this->clientId, newTexture});
    this->spriteHue = hue;
}
