#include "Textures.h"

Textures::Textures() {
    this->executablePath = R"(C:\Users\PC\CLionProjects\AstroParty Online\)";
    this->player = new sf::Texture;
    this->player->loadFromFile(this->executablePath + "resources\\textures\\player.png");
    this->playerWeak = new sf::Texture;
    this->playerWeak->loadFromFile(this->executablePath + "resources\\textures\\player_weak.png");
    this->projectile = new sf::Texture;
    this->projectile->loadFromFile(this->executablePath + "resources\\textures\\projectile.png");
    this->powerupReverse = new sf::Texture;
    this->powerupReverse->loadFromFile(this->executablePath + "resources\\textures\\powerup_reverse.png");
    this->powerupLaser = new sf::Texture;
    this->powerupLaser->loadFromFile(this->executablePath + "resources\\textures\\powerup_laser.png");
    this->laserbeam = new sf::Texture;
    this->laserbeam->loadFromFile(this->executablePath + "resources\\textures\\laserbeam.png");
    this->laserbeam->setRepeated(true);
}

Textures::~Textures() {
    delete this->player;
    delete this->playerWeak;
    delete this->projectile;
    delete this->powerupReverse;
    delete this->powerupLaser;
    delete this->laserbeam;
}
