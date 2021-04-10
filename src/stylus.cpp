#include "stylus.h"

Stylus::Stylus(int w, sf::Color i)
{
    ink = i;
    form = Mode::DRAW;
    width = w;
}

void Stylus::setInk(sf::Color i)
{
    ink = i;
}

void Stylus::swapMode(Mode m)
{
    form = m;
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

Stylus::Mode Stylus::getMode()
{
    return form;
}

int Stylus::getWidth()
{
    return width;
}

sf::Color Stylus::getInk()
{
    return ink;
}