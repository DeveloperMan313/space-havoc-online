#include "Textures.h"

Textures::Textures() {
    this->executablePath = R"(C:\Users\PC\CLionProjects\AstroParty Online\)";
    this->player = new sf::Texture;
    this->player->loadFromFile(this->executablePath + "resources\\textures\\player.png");
    this->playerWeak = new sf::Texture;
    this->playerWeak->loadFromFile(this->executablePath + "resources\\textures\\player_weak.png");
    this->projectile = new sf::Texture;
    this->projectile->loadFromFile(this->executablePath + "resources\\textures\\projectile.png");
}

Textures::~Textures() {
    delete this->player;
    delete this->playerWeak;
    delete this->projectile;
}
