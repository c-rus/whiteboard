#pragma once
#include "SFML/Graphics.hpp"
#include "color.h"

//wrapper class for an sfml circle shape object
class Pixel
{
private:
    sf::CircleShape* dot = nullptr;
    Color dye;
    int radius;
    sf::Vector2f location;

public:
    Pixel(sf::Vector2f& loc, int r, Color c);
    Pixel();
    ~Pixel();
    sf::CircleShape& getDot();
    Color& getColor();
    void drop();
    int getRadius();
    void setLocation(sf::Vector2f loc);
    sf::Vector2f& getLocation();
};