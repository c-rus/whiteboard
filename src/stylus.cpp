#include "stylus.h"

Stylus::Stylus(int r, Color c)
{
    ink = c;
    radius = r;
    eRadius = 10;
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
    int* spot = &radius;
    if(form == ERASE)
        spot = &eRadius;

    if(r < MIN_R)
        *spot = MIN_R;
    else if(r > MAX_R)
        *spot = MAX_R;
    else
        *spot = r;
}

Stylus::Mode Stylus::getMode()
{
    return form;
}

int Stylus::getRadius()
{
    if(form == ERASE)
        return eRadius;
    
    return radius;
}

Color& Stylus::getInk()
{
    return ink;
}