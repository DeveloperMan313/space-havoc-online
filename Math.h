#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>

class Math {
public:
    struct triangle {
        sf::Vector2f v1;
        sf::Vector2f v2;
        sf::Vector2f v3;
    };
    struct line {
        float a;
        float b;
        float c;
    };

    static float sign(float x);

    static sf::Vector2f normalizeVector(sf::Vector2f vector);

    static float dotProduct(sf::Vector2f v1, sf::Vector2f v2);

    static float vectorProduct(sf::Vector2f v1, sf::Vector2f v2);

    static float vectorScale(sf::Vector2f v);

    static float distanceToPoint(Math::line line, sf::Vector2f point);

    static sf::Vector2f closestPointOnLine(Math::line line, sf::Vector2f point);

    static bool pointInTriangle(sf::Vector2f point, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);

    static bool segmentsIntersect(sf::Vector2f &p1, sf::Vector2f &p2, sf::Vector2f &p3, sf::Vector2f &p4);

    static bool vectorsEqual(sf::Vector2f &v1, sf::Vector2f &v2, float epsilon = 10e-4);

    static sf::Vector2f linesIntersectionPoint(Math::line line1, Math::line line2);

    static sf::Vector2f polygonSegmentNormal(const std::vector<sf::Vector2f> &polygon, int i);

    static bool pointInPolygon(sf::Vector2f point, const std::vector<sf::Vector2f> &polygon);

    static Math::line linePointsToEquation(sf::Vector2f p1, sf::Vector2f p2);

    static sf::Vector2f rotateVector(sf::Vector2f v, float angle);

    static int randInt(const int min, const int max);

    static sf::Color HSLtoRGB(float H, float S, float L);

    static std::tuple<float, float, float> RGBtoHSL(const sf::Color &color);
};
