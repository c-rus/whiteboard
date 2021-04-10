#pragma once
#include "SFML/Graphics.hpp"

class Stylus
{
private:
    sf::Color ink;
    int width;
    const int MIN_W = 1;
    const int MAX_W = 20;

public:
    Stylus(int w, sf::Color i);
    
    void setInk(sf::Color i);
    void setWidth(int w);
    
    int getWidth();
    sf::Color getInk();
};