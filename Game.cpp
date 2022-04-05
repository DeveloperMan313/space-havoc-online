#include "Game.h"

#include <utility>

Game::Game(bool isServer, const std::string &ip, int port) {
    this->isServer = isServer;
    this->nextRbId = 0;
    this->textures = new Textures;
    this->rotationDir = 1;
    if (this->isServer) {
        this->server = new Server(port, &this->rigidBodies);
        this->client = nullptr;
        this->initField();
    } else {
        this->server = nullptr;
        this->client = new Client(ip, port, &this->rigidBodies, this->textures);
        sf::VideoMode videoMode(1280, 720);
        this->window = new sf::RenderWindow(videoMode, "AstroParty Online");
    }
}

Game::~Game() {
    delete this->window;
    delete this->textures;
    delete this->server;
    delete this->client;
}

void Game::initField() {
    auto *top = new RigidBody(RigidBody::rbType::rectangle, sf::Vector2f(640, 25), 0, 1, 1, 0, 1280, 50);
    this->addRigidBody(top);

    auto *bottom = new RigidBody(RigidBody::rbType::rectangle, sf::Vector2f(640, 695), 0, 1, 1, 0, 1280, 50);
    this->addRigidBody(bottom);

    auto *left = new RigidBody(RigidBody::rbType::rectangle, sf::Vector2f(25, 360), 0, 1, 1, 0, 50, 620);
    this->addRigidBody(left);

    auto *right = new RigidBody(RigidBody::rbType::rectangle, sf::Vector2f(1255, 360), 0, 1, 1, 0, 50, 620);
    this->addRigidBody(right);
}

void Game::update() {
    Server::clientInput input{};
    if (this->isServer) {
        if (this->sendRbUpdateClock.getElapsedTime().asMilliseconds() > 200) {
            Server::msg update{};
            update.type = Server::msgType::updateRbs;
            this->server->pushRbMsg(update);
            this->sendRbUpdateClock.restart();
        }
        this->server->connectClient();
        this->server->receiveInputData();
        this->server->sendRbData();
        if (this->server->hasClientConnection()) {
            Server::clientConnection connection = this->server->pullConnection();
            if (connection.connected) {
                auto *player = new Player(this->textures);
                player->position = sf::Vector2f(Math::randInt(100, 1180), Math::randInt(100, 620));
                player->clientId = connection.clientId;
                player->setHue((float) Math::randInt(0, 360));
                this->addRigidBody(player);
            } else if (connection.disconnected) {
                for (auto rb : this->rigidBodies) {
                    if (typeid(*rb).name() == std::string("class Player") &&
                        ((Player *) rb)->clientId == connection.clientId) {
                        this->deleteRigidBody(rb->id);
                    }
                }
            }
        }
        for (int i = 0; i < (int) this->rigidBodies.size(); i++) {
            RigidBody *rb = this->rigidBodies[i];
            if (rb->deleted) {
                if (typeid(*rb).name() == std::string("class Player")) {
                    auto *player = (Player *) rb;
                    /*auto *weak = new PlayerWeak(this->textures);
                    weak->position = player->position;
                    weak->clientId = player->clientId;
                    weak->rotation = player->rotation;
                    weak->rotationDir = player->rotationDir;
                    this->addRigidBody(weak);*/
                    if (Math::randInt(0, 100) < 20) {
                        auto *powerUp = new Powerup(this->textures);
                        powerUp->type = Powerup::powerupType::reverse;
                        powerUp->position = player->position;
                        this->addRigidBody(powerUp);
                    }
                } else if (typeid(*rb).name() == std::string("class Powerup")) {
                    auto *powerup = (Powerup *) rb;
                    if (powerup->type == Powerup::powerupType::reverse) {
                        this->rotationDir *= -1;
                        for (RigidBody *rbToReverse : this->rigidBodies) {
                            if (typeid(*rbToReverse).name() == std::string("class Player"))
                                ((Player *) rbToReverse)->rotationDir *= -1;
                        }
                    }
                }
                this->deleteRigidBody(rb->id);
                i--;
            }
        }
        if (this->server->hasClientInput()) {
            Server::clientInput clientInput = this->server->pullInput();
            this->processClientInput(clientInput);
        }
    }
    if (!this->isServer) {
        sf::Event event{};
        if (this->window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    this->client->disconnect();
                    this->window->close();
                case sf::Event::MouseButtonPressed:
                    input.isMouse = true;
                    input.mouseEvent = sf::Event::MouseButtonPressed;
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Left:
                            input.mb = sf::Mouse::Left;
                            break;
                        case sf::Mouse::Right:
                            input.mb = sf::Mouse::Right;
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    input.isMouse = true;
                    input.mouseEvent = sf::Event::MouseButtonReleased;
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Left:
                            input.mb = sf::Mouse::Left;
                            break;
                        case sf::Mouse::Right:
                            input.mb = sf::Mouse::Right;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if (this->client->isConnected()) {
            if (input.isKeyboard || input.isMouse) this->client->pushInput(input);
            this->client->sendInputData();
            this->client->receiveData();
            if (this->client->hasRbState()) {
                Client::rbState state = this->client->pullRbState();
                if (state.added) {
                    if (typeid(*state.rb).name() == std::string("class Player")) {
                        ((Player *) state.rb)->setHue(((Player *) state.rb)->spriteHue);
                    } else if (typeid(*state.rb).name() == std::string("class Powerup")) {
                        ((Powerup *) state.rb)->loadSprite();
                    }
                    this->addRigidBody(state.rb);
                } else if (state.deleted) {
                    this->deleteRigidBody(state.rbId);
                }
            }
        }
        if (!this->client->isConnected()) this->window->close();
    }
}

void Game::render() {
    if (this->isServer) return;
    this->window->clear(sf::Color::Black);
    for (RigidBody *rb : this->rigidBodies) {
        if (rb->type == RigidBody::rbType::circle) {
            RigidBody::hitboxInfo hitbox = rb->hitbox;
            /*sf::CircleShape circle(hitbox.radius);
            circle.setOrigin(sf::Vector2f(hitbox.radius, hitbox.radius));
            circle.setPosition(rb->position);
            circle.setFillColor(sf::Color::Black);
            circle.setOutlineThickness(1);
            circle.setOutlineColor(sf::Color::White);
            this->window->draw(circle);*/
            this->window->draw(rb->sprite);
            sf::Sprite ammoSprite;
            ammoSprite.setTexture(*this->textures->projectile);
            ammoSprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
            ammoSprite.setScale(sf::Vector2f(3, 3));
            ammoSprite.setOrigin(sf::Vector2f(1, 1.5));
            if (typeid(*rb).name() == std::string("class Player") &&
                ((Player *) rb)->clientId == this->client->id()) {
                for (int i = 0; i < ((Player *) rb)->ammo; i++) {
                    ammoSprite.setPosition(
                            rb->position +
                            Math::rotateVector({40, 0}, -(float) M_PI * ((float) 7 / 12 + (float) i / 6)));
                    this->window->draw(ammoSprite);
                }
            }
        }
        if (rb->type == RigidBody::rbType::rectangle) {
            RigidBody::hitboxInfo hitbox = rb->hitbox;
            sf::RectangleShape rectangle(sf::Vector2f(hitbox.width, hitbox.height));
            rectangle.setOrigin(sf::Vector2f(hitbox.width * 0.5, hitbox.height * 0.5));
            rectangle.setPosition(rb->position);
            rectangle.setFillColor(sf::Color(100, 100, 100));
            rectangle.setOutlineThickness(1);
            rectangle.setOutlineColor(sf::Color(100, 100, 100));
            this->window->draw(rectangle);
            this->window->draw(rb->sprite);
        }
    }
    this->window->display();
}

bool Game::isRunning() {
    return this->window->isOpen();
}

void Game::addRigidBody(RigidBody *rigidBody) {
    if (this->isServer) {
        rigidBody->id = this->nextRbId++;
        Server::msg msg{};
        msg.type = Server::msgType::addRb;
        if (typeid(*rigidBody).name() == std::string("class RigidBody"))
            msg.rb = new RigidBody(*rigidBody);
        if (typeid(*rigidBody).name() == std::string("class Player"))
            msg.rb = new Player(*(Player *) rigidBody);
        if (typeid(*rigidBody).name() == std::string("class PlayerWeak"))
            msg.rb = new PlayerWeak(*(PlayerWeak *) rigidBody);
        if (typeid(*rigidBody).name() == std::string("class Projectile"))
            msg.rb = new Projectile(*(Projectile *) rigidBody);
        if (typeid(*rigidBody).name() == std::string("class Powerup"))
            msg.rb = new Powerup(*(Powerup *) rigidBody);
        msg.rb->id = rigidBody->id;
        this->server->pushRbMsg(msg);
    }
    this->rigidBodies.push_back(rigidBody);
}

void Game::deleteRigidBody(int id) {
    int idx = -1;
    for (int i = 0; i < (int) this->rigidBodies.size(); i++) {
        if (this->rigidBodies[i]->id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return;
    if (this->isServer) {
        Server::msg msg{};
        msg.type = Server::msgType::deleteRb;
        msg.rbId = id;
        this->server->pushRbMsg(msg);
    }
    if (!this->isServer) {
        RigidBody *rb = rigidBodies[idx];
        if ((typeid(*rb).name() == std::string("class Player") &&
             (*(Player *) rb).clientId == this->client->id()) ||
            (typeid(*rb).name() == std::string("class PlayerWeak") &&
             (*(PlayerWeak *) rb).clientId == this->client->id())) {
            this->client->disconnect();
            this->window->close();
        }
    }
    delete this->rigidBodies[idx];
    this->rigidBodies.erase(this->rigidBodies.begin() + idx);
}

void Game::physicsUpdate(float timeDelta) {
    for (int i = 0; i < this->rigidBodies.size(); i++) {
        if (rigidBodies[i]->type == RigidBody::rbType::rectangle) continue;
        RigidBody *rb1 = this->rigidBodies[i];
        if (this->isServer && typeid(*rb1).name() == std::string("class Player") && ((Player *) rb1)->ammo < 3 &&
            ((Player *) rb1)->ammoClock.getElapsedTime().asSeconds() > 1.f) {
            ((Player *) rb1)->ammo++;
            ((Player *) rb1)->ammoClock.restart();
        }
        for (int j = 0; j < this->rigidBodies.size(); j++) {
            if (j == i) continue;
            RigidBody *rb2 = this->rigidBodies[j];
            RigidBody::IntersectionInfo intersection = rb1->getIntersectionInfo(*rb2);
            if (intersection.median.x != INFINITY) {
                sf::Vector2f normal = intersection.normal;
                sf::Vector2f rv = rb2->velocity - rb1->velocity;
                float velAlongNormal = Math::dotProduct(rv, normal);
                rb1->position = rb1->position - normal * intersection.depth * 0.8f;
                if (velAlongNormal > 0) continue;
                float e = std::min(rb1->elasticity, rb2->elasticity);
                float scalar = -(1 + e) * velAlongNormal;
                scalar /= 1 / rb1->mass + 1 / rb2->mass;
                sf::Vector2f impulse = scalar * normal;
                rb1->velocity = rb1->velocity - 1 / rb1->mass * impulse;
                rb2->velocity = rb2->velocity + 1 / rb2->mass * impulse;
                if (this->isServer) {
                    rb1->processCollision(rb2);
                    if (rb2->type == RigidBody::rbType::circle) rb2->processCollision(rb1);
                }
            }
        }
    }
    for (auto rb : this->rigidBodies) {
        rb->physicsStep(timeDelta);
        if (this->isServer && Math::vectorScale(rb->position - sf::Vector2f(400, 300)) > 2000) rb->deleted = true;
    }
}

void Game::processClientInput(Server::clientInput input) {
    Player *player = nullptr;
    for (RigidBody *rb : this->rigidBodies) {
        if ((typeid(*rb).name() == std::string("class Player") ||
             typeid(*rb).name() == std::string("class PlayerWeak")) && ((Player *) rb)->clientId == input.clientId) {
            player = (Player *) rb;
            break;
        }
    }
    if (player == nullptr) return;
    if (input.isMouse) {
        if (input.mouseEvent == sf::Event::MouseButtonPressed) {
            Projectile *projectile;
            switch (input.mb) {
                case sf::Mouse::Left:
                    if (player->LMBelapsed.getElapsedTime().asMilliseconds() < 300) player->jump();
                    player->LMBelapsed.restart();
                    player->rotationDir = this->rotationDir;
                    break;
                case sf::Mouse::Right:
                    if (player->ammo > 0) {
                        player->ammo--;
                        player->ammoClock.restart();
                        projectile = new Projectile(this->textures);
                        projectile->position =
                                player->position + Math::rotateVector(sf::Vector2f(30, 0), player->rotation);
                        projectile->rotation = player->rotation;
                        projectile->velocity = Math::rotateVector(sf::Vector2f(1000, 0), player->rotation);
                        projectile->clientId = player->clientId;
                        this->addRigidBody(projectile);
                    }
                    break;
                default:
                    break;
            }
        } else if (input.mouseEvent == sf::Event::MouseButtonReleased) {
            switch (input.mb) {
                case sf::Mouse::Left:
                    player->rotationDir = 0;
                    break;
                default:
                    break;
            }
        }
    }
    Server::msg msg{};
    msg.type = Server::msgType::updateRbs;
    this->server->pushRbMsg(msg);
}
