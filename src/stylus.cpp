#include "stylus.h"

Stylus::Stylus(int w, Color c)
{
    ink = c;
    form = Mode::DRAW;
    width = w;
}

void Stylus::setInk(Color c)
{
    ink = c;
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

Color& Stylus::getInk()
{
    return ink;
}