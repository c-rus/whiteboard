#pragma once
#include <stack>
#include <list>
#include <SFML/Graphics.hpp>

//a squiggle is made from a pair of points, with lines drawn from each successive pair
class Squiggle
{
private:
    std::stack<sf::Vector2i> points;
    std::list<sf::RectangleShape*> lines;
    int mag;
public:
    Squiggle(sf::Vector2i& start, int size, sf::Color color);

    void addPoint(sf::Vector2i& p, int size, sf::Color color);
    void draw(sf::RenderWindow&  win);
    void move(sf::Vector2i& offset);
    void zoom(int magnify, sf::Vector2i& mPoint);
};