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
    };

private:
    Color ink;
    int width;
    const int MIN_W = 1;
    const int MAX_W = 20;
    Mode form;

public:
    Stylus(int w, Color c);
    
    void setInk(Color c);
    void setWidth(int w);
    void swapMode(Mode m);

    Mode getMode();
    int getWidth();
    Color& getInk();
};