#pragma once
#include "box.h"
#include "SFML/Graphics.hpp"

class Clickable
{
private:
    Box bounds;
    sf::RectangleShape button;

public:
    Clickable(int x, int y, int w, int h)
    {
        button.setPosition(x, y);
        button.setSize(sf::Vector2f(w, h));
        button.setFillColor(sf::Color(10,10,10));
        bounds = Box(x, y, w, h);
    };

    void draw(sf::RenderTexture& surf)
    {
        //TODO: stop drawing every frame, only draw when needing to refresh for opacity look
        surf.draw(button);
    };

    Box& getBounds()
    {
        return bounds;
    };

    void highlight(sf::Vector2i& mLoc)
    {
        Box mouseBounds(mLoc.x, mLoc.y, 0, 0);
        if (getBounds().contains(mouseBounds)) button.setFillColor(sf::Color(10,255,10));
        else button.setFillColor(sf::Color(10,10,10)); 
    }
};