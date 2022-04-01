#include "Client.h"

Client::Client(const std::string& ip, const int port, std::vector<RigidBody *> *rigidBodies,
               Textures *textures) {
    this->socket.connect(ip, port);
    this->clientIsConnected = true;
    this->socket.setBlocking(false);
    this->currentSendPacket = new sf::Packet();
    this->currentReceivePacket = new sf::Packet();
    this->rigidBodies = rigidBodies;
    this->textures = textures;
}

Client::~Client() {
    delete this->currentSendPacket;
    delete this->currentReceivePacket;
}

void Client::pushInput(const Server::clientInput &input) {
    this->inputQueue.push_back(input);
}

void Client::sendInputData() {
    if (!this->clientIsConnected) return;
    if (this->currentSendPacket == nullptr) {
        if (this->inputQueue.empty()) return;
        this->currentSendPacket = new sf::Packet();
        Server::clientInput input = this->inputQueue.front();
        this->inputQueue.pop_front();
        *this->currentSendPacket << (int) Server::msgType::clientInput;
        *this->currentSendPacket << input.isKeyboard << input.keyboardEvent << input.key;
        *this->currentSendPacket << input.isMouse << input.mouseEvent << input.mb;
    }
    sf::Socket::Status status = this->socket.send(*this->currentSendPacket);
    if (status == sf::Socket::Done) {
        delete this->currentSendPacket;
        this->currentSendPacket = nullptr;
    } else if (status == sf::Socket::Disconnected) {
        std::cout << "server disconnected\n";
        this->clientIsConnected = false;
    }
}

void Client::receiveData() {
    if (!this->clientIsConnected) return;
    sf::Socket::Status status = this->socket.receive(*this->currentReceivePacket);
    if (status == sf::Socket::Done) {
        int msgType;
        int rbType;
        int updateId;
        int rbCount;
        std::string rbClass;
        RigidBody *newRb = nullptr;
        *this->currentReceivePacket >> msgType;
        Client::rbState state;
        switch ((Server::msgType) msgType) {
            case Server::msgType::updateRbs:
                *this->currentReceivePacket >> rbCount;
                for (int i = 0; i < rbCount; i++) {
                    *this->currentReceivePacket >> rbClass;
                    if (rbClass == std::string("class RigidBody")) {
                        newRb = new RigidBody();
                    } else if (rbClass == std::string("class Player")) {
                        newRb = new Player(this->textures);
                    } else if (rbClass == std::string("class PlayerWeak")) {
                        newRb = new PlayerWeak(this->textures);
                    } else if (rbClass == std::string("class Projectile")) {
                        newRb = new Projectile(this->textures);
                    }
                    *this->currentReceivePacket >> updateId;
                    *this->currentReceivePacket >> newRb->position.x;
                    *this->currentReceivePacket >> newRb->position.y;
                    *this->currentReceivePacket >> newRb->velocity.x;
                    *this->currentReceivePacket >> newRb->velocity.y;
                    *this->currentReceivePacket >> newRb->mass;
                    *this->currentReceivePacket >> newRb->elasticity;
                    *this->currentReceivePacket >> newRb->rotation;
                    *this->currentReceivePacket >> rbType;
                    newRb->type = (RigidBody::rbType) rbType;
                    *this->currentReceivePacket >> newRb->hitbox.radius;
                    *this->currentReceivePacket >> newRb->hitbox.width;
                    *this->currentReceivePacket >> newRb->hitbox.height;
                    if (rbClass == std::string("class Player")) {
                        *this->currentReceivePacket >> ((Player *) newRb)->velocityScalar;
                        *this->currentReceivePacket >> ((Player *) newRb)->rotationDir;
                    }
                    for (RigidBody *rb : *this->rigidBodies) {
                        if (rb->id == updateId) {
                            rb->position.x = newRb->position.x;
                            rb->position.y = newRb->position.y;
                            rb->velocity.x = newRb->velocity.x;
                            rb->velocity.y = newRb->velocity.y;
                            rb->mass = newRb->mass;
                            rb->elasticity = newRb->elasticity;
                            rb->rotation = newRb->rotation;
                            rb->type = newRb->type;
                            rb->hitbox.radius = newRb->hitbox.radius;
                            rb->hitbox.width = newRb->hitbox.width;
                            rb->hitbox.height = newRb->hitbox.height;
                            if (rbClass == std::string("class Player")) {
                                ((Player *) rb)->velocityScalar = ((Player *) newRb)->velocityScalar;
                                ((Player *) rb)->rotationDir = ((Player *) newRb)->rotationDir;
                            } else if (rbClass == std::string("class PlayerWeak")) {
                                ((PlayerWeak *) rb)->velocityScalar = ((PlayerWeak *) newRb)->velocityScalar;
                                ((PlayerWeak *) rb)->rotationDir = ((PlayerWeak *) newRb)->rotationDir;
                            }
                            break;
                        }
                    }
                    newRb = nullptr;
                }
                break;
            case Server::msgType::addRb:
                *this->currentReceivePacket >> rbClass;
                if (rbClass == std::string("class RigidBody")) {
                    newRb = new RigidBody();
                } else if (rbClass == std::string("class Player")) {
                    newRb = new Player(this->textures);
                } else if (rbClass == std::string("class PlayerWeak")) {
                    newRb = new PlayerWeak(this->textures);
                } else if (rbClass == std::string("class Projectile")) {
                    newRb = new Projectile(this->textures);
                }
                *this->currentReceivePacket >> newRb->id;
                *this->currentReceivePacket >> newRb->position.x;
                *this->currentReceivePacket >> newRb->position.y;
                *this->currentReceivePacket >> newRb->velocity.x;
                *this->currentReceivePacket >> newRb->velocity.y;
                *this->currentReceivePacket >> newRb->mass;
                *this->currentReceivePacket >> newRb->elasticity;
                *this->currentReceivePacket >> newRb->rotation;
                *this->currentReceivePacket >> rbType;
                newRb->type = (RigidBody::rbType) rbType;
                *this->currentReceivePacket >> newRb->hitbox.radius;
                *this->currentReceivePacket >> newRb->hitbox.width;
                *this->currentReceivePacket >> newRb->hitbox.height;
                if (rbClass == std::string("class Player")) {
                    *this->currentReceivePacket >> ((Player *) newRb)->velocityScalar;
                    *this->currentReceivePacket >> ((Player *) newRb)->rotationDir;
                    *this->currentReceivePacket >> ((Player *) newRb)->clientId;
                } else if (rbClass == std::string("class PlayerWeak")) {
                    *this->currentReceivePacket >> ((PlayerWeak *) newRb)->velocityScalar;
                    *this->currentReceivePacket >> ((PlayerWeak *) newRb)->rotationDir;
                    *this->currentReceivePacket >> ((PlayerWeak *) newRb)->clientId;
                }
                state.added = true;
                state.rb = newRb;
                this->rbStateQueue.push_back(state);
                break;
            case Server::msgType::deleteRb:
                state.deleted = true;
                *this->currentReceivePacket >> state.rbId;
                this->rbStateQueue.push_back(state);
                break;
            default:
                break;
        }
        delete this->currentReceivePacket;
        this->currentReceivePacket = new sf::Packet();
    } else if (status == sf::Socket::Disconnected) {
        std::cout << "server disconnected\n";
        this->clientIsConnected = false;
    } else if (status != sf::Socket::NotReady) std::cout << "receive error\n";
}

Client::rbState Client::pullRbState() {
    Client::rbState state = this->rbStateQueue.front();
    this->rbStateQueue.pop_front();
    return state;
}

bool Client::hasRbState() {
    return !this->rbStateQueue.empty();
}

bool Client::isConnected() const {
    return this->clientIsConnected;
}
