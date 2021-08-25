#pragma once
#include "SFML/Graphics.hpp"
#include "color.h"

class Stylus
{
public:
    enum Mode
    {
        DRAW,
        ERASE,
        SELECT,
    };

private:
    Color ink;
    int radius;
    int eRadius;
    sf::Cursor style;
    const int MIN_R = 1;
    const int MAX_R = 20;
    Mode form;

public:
    Stylus(int r, Color c);
    
    void setInk(Color c);
    void setRadius(int r);
    sf::Cursor& swapMode(Mode m);

    Mode getMode();
    int getRadius();
    Color& getInk();
};