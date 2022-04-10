#include "PlayerWeak.h"

PlayerWeak::PlayerWeak(Textures *textures) : RigidBody() {
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
    this->spriteHue = 0;
    this->textures = textures;
    this->doPUSpawn = true;
}

PlayerWeak::PlayerWeak(const PlayerWeak &rb) : RigidBody(rb) {
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
    this->spriteHue = rb.spriteHue;
    this->velocityScalar = rb.velocityScalar;
    this->rotationDir = rb.rotationDir;
    this->LMBelapsed = rb.LMBelapsed;
    this->jumpCooldown = rb.jumpCooldown;
    this->doPUSpawn = true;
}

PlayerWeak::~PlayerWeak() {
    this->textures->customPlayerWeakTextures.erase(this->clientId);
}

void PlayerWeak::physicsStep(float timeDelta) {
    this->rotation = this->rotation + 5.4f * (float) this->rotationDir * timeDelta;
    if (this->velocityScalar > 200) this->velocityScalar -= 3000 * timeDelta;
    this->velocity = Math::rotateVector(sf::Vector2f(this->velocityScalar, 0), this->rotation);
    RigidBody::physicsStep(timeDelta);
}

void PlayerWeak::jump() {
    if (this->velocityScalar <= 200) {
        this->velocityScalar = 1200;
    }
}

void PlayerWeak::processCollision(RigidBody *other) {
    if (typeid(*other).name() == std::string("class Projectile") &&
        ((Projectile *) other)->clientId != this->clientId) {
        this->deleted = true;
    }
}

void PlayerWeak::setHue(float hue) {
    this->textures->customPlayerWeakTextures.erase(this->clientId);
    sf::Image image = this->textures->playerWeak->copyToImage();
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
    this->textures->customPlayerWeakTextures.insert({this->clientId, newTexture});
    this->spriteHue = hue;
}
