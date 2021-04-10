#pragma once
#include "SFML/Graphics.hpp"

class Stylus
{
public:
    enum Mode
    {
        DRAW,
        ERASE,
    };

private:
    sf::Color ink;
    int width;
    const int MIN_W = 1;
    const int MAX_W = 20;
    Mode form;

public:
    Stylus(int w, sf::Color i);
    
    void setInk(sf::Color i);
    void setWidth(int w);
    void swapMode(Mode m);

    Mode getMode();
    int getWidth();
    sf::Color getInk();
};