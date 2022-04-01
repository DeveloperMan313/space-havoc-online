#include "Server.h"

Server::Server(const int port, std::vector<RigidBody *> *rigidBodies) {
    this->listener.setBlocking(false);
    this->listener.listen(port);
    this->rigidBodies = rigidBodies;
    this->nextClientId = 0;
}

Server::~Server() {
    for (Server::clientInfo client : this->clients) {
        client.socket->disconnect();
        delete client.socket;
    }
    for (sf::Packet *packet : this->currentSendPackets) delete packet;
    for (sf::Packet *packet : this->currentReceivePackets) delete packet;
}

void Server::connectClient() {
    auto *connectingSocket = new sf::TcpSocket();
    if (this->listener.accept(*connectingSocket) == sf::Socket::Done) {
        Server::clientInfo client{};
        client.socket = connectingSocket;
        client.clientId = this->nextClientId;
        this->clients.push_back(client);
        this->clientPacketQueue.emplace_back();
        this->currentSendPackets.push_back(nullptr);
        this->currentReceivePackets.push_back(nullptr);
        Server::clientConnection msg;
        msg.connected = true;
        msg.clientId = this->nextClientId;
        this->nextClientId++;
        this->connectionQueue.push_back(msg);
        sf::Packet packet;
        packet << client.clientId;
        packet << (int) this->rigidBodies->size();
        connectingSocket->setBlocking(true);
        connectingSocket->send(packet);
        for (RigidBody *rb : *this->rigidBodies) {
            packet = sf::Packet();
            std::string rbClass;
            rbClass = std::string(typeid(*rb).name());
            packet << (int) Server::msgType::addRb;
            packet << rbClass;
            packet << rb->id;
            packet << rb->position.x;
            packet << rb->position.y;
            packet << rb->velocity.x;
            packet << rb->velocity.y;
            packet << rb->mass;
            packet << rb->elasticity;
            packet << rb->rotation;
            packet << (int) rb->type;
            packet << rb->hitbox.radius;
            packet << rb->hitbox.width;
            packet << rb->hitbox.height;
            if (rbClass == std::string("class Player")) {
                packet << ((Player *) rb)->velocityScalar;
                packet << ((Player *) rb)->rotationDir;
                packet << ((Player *) rb)->clientId;
                packet << ((Player *) rb)->ammo;
            }
            sf::Socket::Status status;
            while (true) {
                status = connectingSocket->send(packet);
                if (status == sf::Socket::Status::Done) break;
                else if (status == sf::Socket::Status::Disconnected) {
                    this->disconnectClient((int) this->clients.size() - 1);
                    return;
                }
            }
        }
        connectingSocket->setBlocking(false);
        std::cout << "client " << this->nextClientId - 1 << " connected\n";
    }
}

void Server::disconnectClient(int i) {
    Server::clientConnection msg;
    msg.disconnected = true;
    msg.clientId = clients[i].clientId;
    this->connectionQueue.push_back(msg);
    std::cout << "client " << clients[i].clientId << " disconnected\n";
    this->clients[i].socket->disconnect();
    delete this->clients[i].socket;
    this->clients.erase(this->clients.begin() + i);
    delete this->currentSendPackets[i];
    this->currentSendPackets.erase(this->currentSendPackets.begin() + i);
    delete this->currentReceivePackets[i];
    this->currentReceivePackets.erase(this->currentReceivePackets.begin() + i);
    this->clientPacketQueue.erase(this->clientPacketQueue.begin() + i);
}

void Server::pushRbMsg(const Server::msg msg) {
    if (!this->clients.empty()) this->msgQueue.push_back(msg);
}

Server::clientInput Server::pullInput() {
    Server::clientInput input = this->inputQueue.front();
    this->inputQueue.pop_front();
    return input;
}

Server::clientConnection Server::pullConnection() {
    Server::clientConnection connection = this->connectionQueue.front();
    this->connectionQueue.pop_front();
    return connection;
}

void Server::sendRbData() {
    if (!this->msgQueue.empty()) {
        Server::msg msg = this->msgQueue.front();
        this->msgQueue.pop_front();
        sf::Packet packet;
        std::string rbClass;
        packet << (int) msg.type;
        RigidBody *newRb = nullptr;
        switch (msg.type) {
            case Server::msgType::updateRbs:
                packet << (int) this->rigidBodies->size();
                for (RigidBody *rb : *this->rigidBodies) {
                    rbClass = std::string(typeid(*rb).name());
                    packet << rbClass;
                    packet << rb->id;
                    packet << rb->position.x;
                    packet << rb->position.y;
                    packet << rb->velocity.x;
                    packet << rb->velocity.y;
                    packet << rb->mass;
                    packet << rb->elasticity;
                    packet << rb->rotation;
                    packet << (int) rb->type;
                    packet << rb->hitbox.radius;
                    packet << rb->hitbox.width;
                    packet << rb->hitbox.height;
                    if (rbClass == std::string("class Player")) {
                        packet << ((Player *) rb)->velocityScalar;
                        packet << ((Player *) rb)->rotationDir;
                        packet << ((Player *) rb)->ammo;
                    } else if (rbClass == std::string("class PlayerWeak")) {
                        packet << ((PlayerWeak *) rb)->velocityScalar;
                        packet << ((PlayerWeak *) rb)->rotationDir;
                    }
                }
                break;
            case Server::msgType::addRb:
                newRb = msg.rb;
                rbClass = std::string(typeid(*newRb).name());
                packet << rbClass;
                packet << newRb->id;
                packet << newRb->position.x;
                packet << newRb->position.y;
                packet << newRb->velocity.x;
                packet << newRb->velocity.y;
                packet << newRb->mass;
                packet << newRb->elasticity;
                packet << newRb->rotation;
                packet << (int) newRb->type;
                packet << newRb->hitbox.radius;
                packet << newRb->hitbox.width;
                packet << newRb->hitbox.height;
                if (rbClass == std::string("class Player")) {
                    packet << ((Player *) newRb)->velocityScalar;
                    packet << ((Player *) newRb)->rotationDir;
                    packet << ((Player *) newRb)->clientId;
                    packet << ((Player *) newRb)->ammo;
                } else if (rbClass == std::string("class PlayerWeak")) {
                    packet << ((PlayerWeak *) newRb)->velocityScalar;
                    packet << ((PlayerWeak *) newRb)->rotationDir;
                    packet << ((PlayerWeak *) newRb)->clientId;
                }
                break;
            case Server::msgType::deleteRb:
                packet << msg.rbId;
                break;
            default:
                break;
        }
        delete newRb;
        for (int i = 0; i < this->clients.size(); i++) this->clientPacketQueue[i].push_back(new sf::Packet(packet));
    }
    for (int i = 0; i < this->clients.size(); i++) {
        if (this->currentSendPackets[i] == nullptr) {
            if (this->clientPacketQueue[i].empty()) continue;
            sf::Packet *packet = this->clientPacketQueue[i].front();
            this->clientPacketQueue[i].pop_front();
            this->currentSendPackets[i] = packet;
        }
        sf::Socket::Status status = this->clients[i].socket->send(*this->currentSendPackets[i]);
        if (status == sf::Socket::Done) {
            delete this->currentSendPackets[i];
            this->currentSendPackets[i] = nullptr;
        } else if (status == sf::Socket::Disconnected) {
            this->disconnectClient(i);
            i--;
        }
    }
}

void Server::receiveInputData() {
    for (int i = 0; i < this->clients.size(); i++) {
        if (this->currentReceivePackets[i] == nullptr) this->currentReceivePackets[i] = new sf::Packet();
        sf::Socket::Status status = this->clients[i].socket->receive(*this->currentReceivePackets[i]);
        if (status == sf::Socket::Done) {
            int msgType;
            int keyboardEvent;
            int key;
            int mouseEvent;
            int mb;
            Server::clientInput input{};
            *this->currentReceivePackets[i] >> msgType;
            switch ((Server::msgType) msgType) {
                case Server::msgType::clientInput:
                    *this->currentReceivePackets[i] >> input.isKeyboard;
                    *this->currentReceivePackets[i] >> keyboardEvent;
                    input.keyboardEvent = (sf::Event::EventType) keyboardEvent;
                    *this->currentReceivePackets[i] >> key;
                    input.key = (sf::Keyboard::Key) key;
                    *this->currentReceivePackets[i] >> input.isMouse;
                    *this->currentReceivePackets[i] >> mouseEvent;
                    input.mouseEvent = (sf::Event::EventType) mouseEvent;
                    *this->currentReceivePackets[i] >> mb;
                    input.mb = (sf::Mouse::Button) mb;
                    input.clientId = this->clients[i].clientId;
                    this->inputQueue.push_back(input);
                    break;
                default:
                    break;
            }
            delete this->currentReceivePackets[i];
            this->currentReceivePackets[i] = nullptr;
        } else if (status == sf::Socket::Disconnected) {
            this->disconnectClient(i);
            i--;
        }
    }
}

bool Server::hasClientInput() {
    return !this->inputQueue.empty();
}

bool Server::hasClientConnection() {
    return !this->connectionQueue.empty();
}
