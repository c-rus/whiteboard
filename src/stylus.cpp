#include "stylus.h"

Stylus::Stylus(int r, Color c)
{
    ink = c;
    radius = r;
    swapMode(DRAW);
}

void Stylus::setInk(Color c)
{
    ink = c;
}

sf::Cursor& Stylus::swapMode(Mode m)
{
    form = m;
    switch(form)
    {
        case DRAW:
            style.loadFromSystem(sf::Cursor::Arrow);
            break;
        case ERASE:
            style.loadFromSystem(sf::Cursor::Cross);
            break;
        default:
            style.loadFromSystem(sf::Cursor::Hand);
            break;
    }
    return style;
}

void Stylus::setRadius(int r)
{
    if(r < MIN_R)
        radius = MIN_R;
    else if(r > MAX_R)
        radius = MAX_R;
    else
        radius = r;
}

Stylus::Mode Stylus::getMode()
{
    return form;
}

int Stylus::getRadius()
{
    return radius;
}

Color& Stylus::getInk()
{
    return ink;
}