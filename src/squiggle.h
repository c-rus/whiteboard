#pragma once
#include <stack>
#include <list>
#include <SFML/Graphics.hpp>
#include "color.h"
#include "box.h"
#include <fstream>
#include "pixel.h"

//a squiggle is made from a pair of points, with lines drawn from each successive pair
class Squiggle
{
private:
    sf::Vector2f prev;
    std::list<Pixel*> lines;
    Box bounds;

public:
    Squiggle(sf::Vector2i& start, int width, Color color);
    Squiggle(std::fstream& file); //loading from file
    ~Squiggle();

    bool addPoint(sf::Vector2i& p, int width, Color color);
    void draw(sf::RenderWindow&  win);
    void move(sf::Vector2i& offset);
    void zoom(int magnify, sf::Vector2i& mPoint);
    int count();

    Box& getBounds();
    void save(std::fstream& file);
};