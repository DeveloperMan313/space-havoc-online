#include <iostream>
#include "Game.h"

int main() {
    bool isServer;
    std::string ip;
    int port;
    std::cin >> isServer;
    float stepDuration = 1.f / 200;
    if (isServer) {
        std::cin >> port;
        Game game(isServer, ip, port);
        sf::Clock loopClock;
        while (true) {
            for (int i = 0; i < 20; i++) game.update();
            float timeDelta = loopClock.getElapsedTime().asSeconds();
            if (timeDelta < stepDuration) {
                sf::sleep(sf::seconds(stepDuration - timeDelta));
                timeDelta = loopClock.getElapsedTime().asSeconds();
            }
            loopClock.restart();
            game.physicsUpdate(timeDelta);
        }
    } else {
        std::cin >> ip >> port;
        sf::Clock loopClock;
        while (true) {
            Game game(isServer, ip, port);
            int renderIter = 0;
            while (game.isRunning()) {
                for (int i = 0; i < 20; i++) game.update();
                float timeDelta = loopClock.getElapsedTime().asSeconds();
                if (timeDelta < stepDuration) {
                    sf::sleep(sf::seconds(stepDuration - timeDelta));
                    timeDelta = loopClock.getElapsedTime().asSeconds();
                }
                loopClock.restart();
                game.physicsUpdate(timeDelta);
                if (renderIter == 2) {
                    game.render();
                    renderIter = 0;
                }
                renderIter++;
            }
            system("pause");
        }
    }
}
