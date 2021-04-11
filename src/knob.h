#pragma once
#include "box.h"
#include "SFML/Graphics.hpp"

class Knob
{
private:
    Box bounds;
    sf::RectangleShape button;

public:
    Knob(int x, int y, int w, int h)
    {
        button.setPosition(x, y);
        button.setSize(sf::Vector2f(w, h));
        button.setFillColor(sf::Color(200,125,20));
        bounds = Box(x, y, w, h);
    };

    void draw(sf::RenderTexture& surf)
    {
        surf.draw(button);
    };

    Box& getBounds()
    {
        return bounds;
    };

    void highlight(sf::Vector2i& mLoc)
    {
        Box mouseBounds(mLoc.x, mLoc.y, 0, 0);
        if (getBounds().contains(mouseBounds)) button.setFillColor(sf::Color(255,125,0));
        else button.setFillColor(sf::Color(200,125,20));
    }
};