#include "RigidBody.h"

RigidBody::RigidBody() {
    this->type = RigidBody::rbType::circle;
    this->position = sf::Vector2f(0, 0);
    this->rotation = 0.f;
    this->mass = 1.f;
    this->elasticity = 1.f;
    this->hitbox = {20.f, 0.f, 0.f};
    this->resultantForce = {0, 0};
    this->deleted = false;
    this->id = -1;
}

RigidBody::RigidBody(enum rbType type, sf::Vector2f position, float rotation, float mass, float elasticity,
                     float radius, float width, float height) {
    this->type = type;
    this->position = position;
    this->rotation = rotation;
    this->mass = mass;
    this->elasticity = elasticity;
    this->hitbox.radius = radius;
    this->hitbox.width = width;
    this->hitbox.height = height;
    this->resultantForce = {0, 0};
    this->deleted = false;
    this->id = -1;

    if (this->type == RigidBody::rbType::rectangle) this->mass = INFINITY;
}

RigidBody::~RigidBody() = default;

RigidBody::IntersectionInfo RigidBody::getIntersectionInfo(RigidBody &other) const {
    RigidBody::hitboxInfo info1 = this->hitbox;
    RigidBody::hitboxInfo info2 = other.hitbox;
    if (this->type == RigidBody::rbType::circle && other.type == RigidBody::rbType::circle) {
        float r1 = info1.radius;
        float r2 = info2.radius;
        sf::Vector2f point = {INFINITY, INFINITY};
        sf::Vector2f deltaPos = other.position - this->position;
        float dist = Math::vectorScale(deltaPos);
        sf::Vector2f normal = Math::normalizeVector(deltaPos);
        if (dist < r1 + r2) point = this->position + normal * (r1 - (r1 + r2 - dist) * 1.0f);
        RigidBody::IntersectionInfo result;
        result.median = point;
        result.normal = normal;
        result.depth = r1 + r2 - dist;
        return result;
    } else if (this->type == RigidBody::rbType::rectangle && other.type == RigidBody::rbType::rectangle) {
        RigidBody::IntersectionInfo result;
        return result;
    } else {
        float r;
        float w;
        float h;
        sf::Vector2f circlePos;
        sf::Vector2f rectanglePos;
        if (this->type == RigidBody::rbType::circle) {
            r = info1.radius;
            w = info2.width;
            h = info2.height;
            circlePos = this->position;
            rectanglePos = other.position;
        } else {
            r = info2.radius;
            w = info1.width;
            h = info1.height;
            circlePos = other.position;
            rectanglePos = this->position;
        }
        RigidBody::IntersectionInfo result;
        std::vector<sf::Vector2f> rectPoints =
                {rectanglePos + sf::Vector2f(-w / 2, -h / 2),
                 rectanglePos + sf::Vector2f(w / 2, -h / 2),
                 rectanglePos + sf::Vector2f(-w / 2, h / 2),
                 rectanglePos + sf::Vector2f(w / 2, h / 2)};
        float minDistPoint = INFINITY;
        sf::Vector2f deltaPosPoint;
        for (sf::Vector2f &p : rectPoints) {
            float dist = Math::vectorScale(p - circlePos);
            if (dist < r && dist < minDistPoint) {
                deltaPosPoint = p - circlePos;
                minDistPoint = dist;
            }
        }
        float top = 10e6;
        if (rectanglePos.y - h / 2 - circlePos.y < r && circlePos.y < rectanglePos.y &&
            std::abs(circlePos.x - rectanglePos.x) < w / 2)
            top = rectanglePos.y - h / 2 - circlePos.y;
        float bottom = 10e6;
        if (circlePos.y - (rectanglePos.y + h / 2) < r && circlePos.y > rectanglePos.y &&
            std::abs(circlePos.x - rectanglePos.x) < w / 2)
            bottom = circlePos.y - (rectanglePos.y + h / 2);
        float left = 10e6;
        if (rectanglePos.x - w / 2 - circlePos.x < r && circlePos.x < rectanglePos.x &&
            std::abs(circlePos.y - rectanglePos.y) < h / 2)
            left = rectanglePos.x - w / 2 - circlePos.x;
        float right = 10e6;
        if (circlePos.x - (rectanglePos.x + w / 2) < r && circlePos.x > rectanglePos.x &&
            std::abs(circlePos.y - rectanglePos.y) < h / 2)
            right = circlePos.x - (rectanglePos.x + w / 2);
        float minDistEdge = std::min(std::min(top, bottom), std::min(left, right));
        if (minDistPoint < minDistEdge) {
            result.normal = Math::normalizeVector(deltaPosPoint);
            result.median = result.normal * (minDistPoint + r) * 0.5f;
            result.depth = r - minDistPoint;
        } else {
            if (minDistEdge > 10e5) {
                result.normal = sf::Vector2f(0, 0);
                result.median = sf::Vector2f(INFINITY, INFINITY);
                result.depth = INFINITY;
                return result;
            }
            if (minDistEdge == top) {
                result.normal = sf::Vector2f(0, 1);
                result.median = sf::Vector2f(circlePos.x, (circlePos.y + r + rectanglePos.y - h / 2) * 0.5f);
                result.depth = r - ((rectanglePos.y - h / 2) - circlePos.y);
            } else if (minDistEdge == bottom) {
                result.normal = sf::Vector2f(0, -1);
                result.median = sf::Vector2f(circlePos.x, (circlePos.y - r + rectanglePos.y + h / 2) * 0.5f);
                result.depth = r - (circlePos.y - (rectanglePos.y + h / 2));
            } else if (minDistEdge == left) {
                result.normal = sf::Vector2f(1, 0);
                result.median = sf::Vector2f((circlePos.x + r + rectanglePos.x - w / 2) * 0.5f, circlePos.y);
                result.depth = r - ((rectanglePos.x - w / 2) - circlePos.x);
            } else if (minDistEdge == right) {
                result.normal = sf::Vector2f(-1, 0);
                result.median = sf::Vector2f((circlePos.x - r + rectanglePos.x + w / 2) * 0.5f, circlePos.y);
                result.depth = r - (circlePos.x - (rectanglePos.x + w / 2));
            }
        }
        return result;
    }
}

void RigidBody::physicsStep(float timeDelta) {
    this->sprite.setPosition(this->position);
    this->sprite.setRotation(this->rotation * 180.f / 3.1415f);
    if (this->type == RigidBody::rbType::rectangle) return;
    this->velocity += this->resultantForce / this->mass;
    this->position += this->velocity * timeDelta;
    this->resultantForce = {0, 0};
}

void RigidBody::applyForce(sf::Vector2f force) {
    if (this->type == RigidBody::rbType::rectangle) return;
    this->resultantForce += force;
}

void RigidBody::processCollision(RigidBody *other) {

}

RigidBody::RigidBody(const RigidBody &rb) {
    this->type = rb.type;
    this->hitbox = rb.hitbox;
    this->position = rb.position;
    this->rotation = rb.rotation;
    this->mass = rb.mass;
    this->elasticity = rb.elasticity;
    this->hitbox.radius = rb.hitbox.radius;
    this->hitbox.width = rb.hitbox.width;
    this->hitbox.height = rb.hitbox.height;
    this->resultantForce = rb.resultantForce;
    this->deleted = rb.deleted;
    this->id = rb.id;
}
