#include "pixel.h"

Pixel::Pixel(sf::Vector2f& loc, int r, Color c)
{
    dot = new sf::CircleShape(r);
    dot->setFillColor(c.getSFColor());
    dot->setPosition(loc);
}

Pixel::Pixel()
{
    dot = new sf::CircleShape();
}

Pixel::~Pixel()
{
    delete dot;
}

sf::CircleShape& Pixel::getDot()
{
    return *dot;
}

std::string Pixel::getKey()
{
    return std::to_string(dot->getPosition().x) + "," + std::to_string(dot->getPosition().y);
}