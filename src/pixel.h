#pragma once
#include "SFML/Graphics.hpp"
#include "color.h"

//wrapper class for an sfml circle shape object
class Pixel
{
private:
    sf::CircleShape* dot = nullptr;
    
public:
    Pixel(sf::Vector2f& loc, int r, Color c);
    Pixel();
    ~Pixel();
    sf::CircleShape& getDot();
    std::string getKey();
};