#include "Math.h"

float Math::sign(float x) {
    if (x > 0) return 1;
    else if (x == 0) return 0;
    else return -1;
}

sf::Vector2f Math::normalizeVector(sf::Vector2f vector) {
    return vector / std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Math::dotProduct(sf::Vector2f v1, sf::Vector2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float Math::vectorProduct(sf::Vector2f v1, sf::Vector2f v2) {
    return v1.x * v2.y - v2.x * v1.y;
}

float Math::vectorScale(sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

float Math::distanceToPoint(Math::line line, sf::Vector2f point) {
    float a = line.a;
    float b = line.b;
    float c = line.c;
    float x = point.x;
    float y = point.y;
    return std::abs(a * x + b * y + c) / std::sqrt(a * a + b * b);
}

sf::Vector2f Math::closestPointOnLine(Math::line line, sf::Vector2f point) {
    float a = line.a;
    float b = line.b;
    float c = line.c;
    float x = point.x;
    float y = point.y;
    float divider = a * a + b * b;
    return {(b * (b * x - a * y) - a * c) / divider, (a * (-b * x + a * y) - b * c) / divider};
}

bool Math::pointInTriangle(sf::Vector2f point, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
    float s1 = Math::vectorProduct(b - a, point - a);
    float s2 = Math::vectorProduct(c - b, point - b);
    float s3 = Math::vectorProduct(a - c, point - c);
    return (s1 >= 0 && s2 >= 0 && s3 >= 0) || (s1 < 0 && s2 < 0 && s3 < 0);
}

bool Math::segmentsIntersect(sf::Vector2f &p1, sf::Vector2f &p2, sf::Vector2f &p3, sf::Vector2f &p4) {
    float x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y, x3 = p3.x, y3 = p3.y, x4 = p4.x, y4 = p4.y;
    return Math::sign((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)) !=
           Math::sign((x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1))
           && Math::sign((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) !=
              Math::sign((x4 - x3) * (y2 - y3) - (y4 - y3) * (x2 - x3));
}

bool Math::vectorsEqual(sf::Vector2f &v1, sf::Vector2f &v2, float epsilon) {
    return Math::vectorScale(v1 - v2) < epsilon;
}

sf::Vector2f Math::linesIntercectionPoint(Math::line line1, Math::line line2) {
    float a1 = line1.a;
    float b1 = line1.b;
    float c1 = line1.c;
    float a2 = line2.a;
    float b2 = line2.b;
    float c2 = line2.c;
    sf::Vector2f point = {INFINITY, INFINITY};
    if (a1 * b2 != a2 * b1) {
        point.x = (c2 / b2 - c1) / (a1 - b1 * a2 / b2);
        point.y = -(a2 * point.x + c2) / b2;
    }
    return point;
}

sf::Vector2f Math::polygonSegmentNormal(const std::vector<sf::Vector2f> &polygon, int i) {
    sf::Vector2f v = Math::normalizeVector(polygon[((size_t) i + 1) % polygon.size()] - polygon[i]);
    sf::Vector2f normal;
    normal.x = v.y;
    normal.y = -v.x;
    return normal;
}

bool Math::pointInPolygon(sf::Vector2f point, const std::vector<sf::Vector2f> &polygon) {
    float minDistEdge = INFINITY;
    float minDistPoint = INFINITY;
    sf::Vector2f closestEdgePoint;
    sf::Vector2f closestPoint;
    int closestEdgeIdx = 0;
    int closestPointIdx = 0;
    for (int i = 0; i < (int) polygon.size(); i++) {
        sf::Vector2f p1 = polygon[i];
        sf::Vector2f p2 = polygon[(i + 1) % (int) polygon.size()];
        Math::line l = Math::linePointsToEquation(p1, p2);
        sf::Vector2f closePoint = Math::closestPointOnLine(l, point);
        float dist = Math::vectorScale(point - closePoint);
        if (dist < minDistEdge &&
            !(closePoint.x < std::min(p1.x, p2.x) ||
              closePoint.x > std::max(p1.x, p2.x) ||
              closePoint.y < std::min(p1.y, p2.y) ||
              closePoint.y > std::max(p1.y, p2.y))) {
            minDistEdge = dist;
            closestEdgePoint = closePoint;
            closestEdgeIdx = i;
        }
    }
    for (int i = 0; i < (int) polygon.size(); i++) {
        float dist = Math::vectorScale(point - polygon[i]);
        if (dist < minDistPoint) {
            minDistPoint = dist;
            closestPoint = polygon[i];
            closestPointIdx = i;
        }
    }
    if (minDistEdge <= minDistPoint)
        return Math::dotProduct(closestEdgePoint - point, Math::polygonSegmentNormal(polygon, closestEdgeIdx)) > 0;
    int prevIdx = (closestPointIdx > 0) ? closestPointIdx - 1 : closestPointIdx - 1 + (int) polygon.size();
    return Math::dotProduct(closestPoint - point, (Math::polygonSegmentNormal(polygon, prevIdx) +
                                                   Math::polygonSegmentNormal(polygon, closestPointIdx)) * 0.5f) > 0;
}

Math::line Math::linePointsToEquation(sf::Vector2f p1, sf::Vector2f p2) {
    Math::line l{};
    if (p1.x - p2.x != 0) {
        float k = (p1.y - p2.y) / (p1.x - p2.x);
        float b = p1.y - p1.x * k;
        l.a = -k;
        l.b = 1;
        l.c = -b;
    } else {
        l.a = 1;
        l.b = 0;
        l.c = -p1.x;
    }
    return l;
}

sf::Vector2f Math::rotateVector(sf::Vector2f v, float angle) {
    float x = v.x;
    float y = v.y;
    float sin = std::sin(angle);
    float cos = std::cos(angle);
    return {x * cos - y * sin, x * sin + y * cos};
}
