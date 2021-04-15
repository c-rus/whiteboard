#include "pixel.h"

Pixel::Pixel(sf::Vector2f& loc, int r, Color c)
{
    dot = new sf::CircleShape(r);
    dot->setFillColor(c.getSFColor());
    dot->setPosition(loc);
    dye = c;
    radius = r;
    location = loc;
}

Pixel::Pixel()
{
    dot = new sf::CircleShape();
    radius = 0;
}

Pixel::~Pixel()
{
    drop();
}

void Pixel::setColor(Color& c)
{
    dye = c;
    dot->setFillColor(dye.getSFColor());
}

void Pixel::setLocation(sf::Vector2f loc)
{
    if(dot != nullptr)
        dot->setPosition(loc);
    location = loc;
}

void Pixel::drop()
{
    if(dot != nullptr)
    {
        delete dot;
        dot = nullptr;
    }
}

Color& Pixel::getColor()
{
    return dye;
}

int Pixel::getRadius()
{
    return radius;
}

sf::Vector2f& Pixel::getLocation()
{
    return location;
}

sf::CircleShape& Pixel::getDot()
{
    return *dot;
}