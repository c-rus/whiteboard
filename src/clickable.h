#pragma once
#include "box.h"
#include "SFML/Graphics.hpp"

class Clickable
{
private:
    Box bounds;
    sf::Sprite sp;
    sf::Texture* tx; //fix

public:
    Clickable(int x, int y, int w, int h)
    {
        tx = new sf::Texture();
        tx->loadFromFile("./assets/reset.png");
        sp.setTexture(*tx);
        sp.setPosition(x, y);
        sp.setTextureRect(sf::IntRect(0, 0, 64, 64));
        bounds = Box(x, y, w, h);
    };

    void draw(sf::RenderTexture& surf)
    {
        //TODO: stop drawing every frame, only draw when needing to refresh for opacity look
        surf.draw(sp);
    };

    Box& getBounds()
    {
        return bounds;
    };

    bool highlight(sf::Vector2i& mLoc)
    {
        Box mouseBounds(mLoc.x, mLoc.y, 0, 0);
        if (getBounds().contains(mouseBounds)) 
        {
            sp.setTextureRect(sf::IntRect(0, 64, 64, 64));
            return true;
        }
        else 
            sp.setTextureRect(sf::IntRect(0, 0, 64, 64));
            return false;
    }
};