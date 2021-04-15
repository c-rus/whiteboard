#pragma once
#include <stack>
#include <list>
#include <fstream>
#include <math.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include "color.h"
#include "box.h"
#include "pixel.h"

//a squiggle is made from a pair of points, with lines drawn from each successive pair
class Squiggle
{
private:
    sf::Vector2f prev;
    std::list<Pixel*> lines;
    std::list<std::pair<sf::Vector2i, unsigned char>> points;

    sf::RenderTexture* rt = nullptr;
    sf::Sprite* sp = nullptr;

    Box bounds;
    int scalar;
    bool optimized;

public:
    Squiggle(sf::Vector2i& start, int radius, Color color);
    Squiggle(std::fstream& file); //loading from file
    ~Squiggle();

    bool addPoint(sf::Vector2i& p, int radius, Color color, bool fromLoad=false);
    void draw(sf::RenderWindow&  win);
    void move(sf::Vector2i& offset);
    void zoom(int scale, sf::Vector2f& origin);
    int count();
    void compress();

    Box& getBounds();
    void save(std::fstream& file);
};