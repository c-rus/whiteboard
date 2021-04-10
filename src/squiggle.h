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
    //TODO: create a box to encapsulate the squiggle's boundaries
    int width;
    const int MAX_W = sf::VideoMode::getDesktopMode().width;
    const int MAX_H = sf::VideoMode::getDesktopMode().height;
public:
    Squiggle(sf::Vector2i& start, int width, sf::Color color);

    void addPoint(sf::Vector2i& p, int width, sf::Color color);
    void draw(sf::RenderWindow&  win);
    void move(sf::Vector2i& offset);
    void zoom(int magnify, sf::Vector2i& mPoint);
    int count();
};