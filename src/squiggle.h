#pragma once
#include <stack>
#include <list>
#include <SFML/Graphics.hpp>
#include "box.h"

//a squiggle is made from a pair of points, with lines drawn from each successive pair
class Squiggle
{
private:
    std::stack<sf::Vector2i> points;
    std::list<sf::RectangleShape*> lines;
    Box bounds;

public:
    Squiggle(sf::Vector2i& start, int width, sf::Color color);

    bool addPoint(sf::Vector2i& p, int width, sf::Color color);
    void draw(sf::RenderWindow&  win);
    void move(sf::Vector2i& offset);
    void zoom(int magnify, sf::Vector2i& mPoint);
    int count();

    Box& getBounds();
};