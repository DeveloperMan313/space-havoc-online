#include "Math.h"

float Math::sign(float x) {
    if (x > 0) return 1;
    else if (x == 0) return 0;
    else return -1;
}

sf::Vector2f Math::normalizeVector(sf::Vector2f vector) {
    if (Math::vectorsEqual(vector, sf::Vector2f(0, 0))) return {1, 0};
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

bool Math::segmentsIntersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4) {
    float x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y, x3 = p3.x, y3 = p3.y, x4 = p4.x, y4 = p4.y;
    return Math::sign((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)) !=
           Math::sign((x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1))
           && Math::sign((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) !=
              Math::sign((x4 - x3) * (y2 - y3) - (y4 - y3) * (x2 - x3));
}

bool Math::vectorsEqual(sf::Vector2f v1, sf::Vector2f v2, float epsilon) {
    return Math::vectorScale(v1 - v2) < epsilon;
}

sf::Vector2f Math::linesIntersectionPoint(Math::line line1, Math::line line2) {
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

int Math::randInt(const int min, const int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1); // NOLINT(cert-msc30-c, cert-msc50-cpp)
}

// HSL/HSV
// H Hue component, range: [0.f, 360.f)
// S Saturation component, range: [0.f, 1.f)
// L Lightness component, range: [0.f, 1.f)

// sf::Color
// r Red component, range: [0, 255]
// g Green component, range: [0, 255]
// b Blue component, range: [0, 255]

sf::Color Math::HSLtoRGB(float H, float S, float L) {
    float C = (1 - std::fabs(2 * L - 1)) * S; // Chroma
    float HPrime = H / 60; // H'
    float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
    float M = L - C / 2;

    float R = 0.f;
    float G = 0.f;
    float B = 0.f;

    switch (static_cast<int>(HPrime)) {
        case 0:
            R = C;
            G = X;
            break; // [0, 1)
        case 1:
            R = X;
            G = C;
            break; // [1, 2)
        case 2:
            G = C;
            B = X;
            break; // [2, 3)
        case 3:
            G = X;
            B = C;
            break; // [3, 4)
        case 4:
            R = X;
            B = C;
            break; // [4, 5)
        case 5:
            R = C;
            B = X;
            break; // [5, 6)
    }

    R += M;
    G += M;
    B += M;

    sf::Color color;
    color.r = static_cast<sf::Uint8>(std::round(R * 255));
    color.g = static_cast<sf::Uint8>(std::round(G * 255));
    color.b = static_cast<sf::Uint8>(std::round(B * 255));

    return color;
}

std::tuple<float, float, float> Math::RGBtoHSL(const sf::Color color) {
    float R = (float) color.r / 255.f;
    float G = (float) color.g / 255.f;
    float B = (float) color.b / 255.f;

    float M = std::max({R, G, B});
    float m = std::min({R, G, B});
    float C = M - m; // Chroma

    float H = 0.f; // Hue
    float S = 0.f; // Saturation
    float L = 0.f; // Lightness

    if (C != 0.f) {
        if (M == R)
            H = std::fmod(((G - B) / C), 6.f);
        else if (M == G)
            H = ((B - R) / C) + 2;
        else if (M == B)
            H = ((R - G) / C) + 4;

        H *= 60;
    }

    if (H < 0.f)
        H += 360;

    L += (M + m) / 2;

    if (L != 1.f && L != 0.f)
        S = C / (1 - std::fabs(2 * L - 1));

    return {H, S, L};
}
