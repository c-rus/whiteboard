#include "stylus.h"

Stylus::Stylus(int w, sf::Color i)
{
    ink = i;
    width = w;
}

void Stylus::setInk(sf::Color i)
{
    ink = i;
}

void Stylus::setWidth(int w)
{
    if(w < MIN_W)
        width = MIN_W;
    else if(w > MAX_W)
        width = MAX_W;
    else
        width = w;
}

int Stylus::getWidth()
{
    return width;
}

sf::Color Stylus::getInk()
{
    return ink;
}