#pragma once

#include <SFML/Graphics.hpp>
#include <string.h>

class Textures {
public:
    std::string executablePath;

    sf::Texture *player;
    sf::Texture *playerWeak;
    sf::Texture *projectile;

    Textures();

    ~Textures();
};
