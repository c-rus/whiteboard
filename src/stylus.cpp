#include "stylus.h"

Stylus::Stylus(int w, Color c)
{
    ink = c;
    width = w;
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

Color& Stylus::getInk()
{
    return ink;
}