#pragma once

#include <SFML/Graphics.hpp>
#include <cstring>
#include <unordered_map>

class Textures {
public:
    std::string executablePath;

    sf::Texture *player;
    sf::Texture *playerWeak;
    sf::Texture *projectile;
    sf::Texture *powerupReverse;

    std::unordered_map<int, sf::Texture *> customPlayerTextures;

    Textures();

    ~Textures();
};
