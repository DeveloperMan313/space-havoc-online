#pragma once

#include <SFML/Graphics.hpp>
#include <string.h>
#include <unordered_map>

class Textures {
public:
    std::string executablePath;

    sf::Texture *player;
    sf::Texture *playerWeak;
    sf::Texture *projectile;

    std::unordered_map<int, sf::Texture *> customPlayerTextures;

    Textures();

    ~Textures();
};
