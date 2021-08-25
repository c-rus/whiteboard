#pragma once
#include "box.h"
#include "SFML/Graphics.hpp"

class Clickable
{
private:
    Box bounds;
    sf::Sprite sp;
    sf::Text t;
    sf::Texture* tx;
    sf::Font* ft;
    bool tipVisible;

public:
    Clickable(int x, int y, int w, int h)
    {
        tx = new sf::Texture();
        tx->loadFromFile("./assets/reset.png");
        ft = new sf::Font();
        ft->loadFromFile("./assets/SF-Pro.otf");
        
        sp.setTexture(*tx);
        sp.setPosition(x, y);
        sp.setTextureRect(sf::IntRect(0, 0, 64, 64));
    
        t.setFont(*ft);
        t.setFillColor(sf::Color::White);
        t.setString("recycle");

        bounds = Box(x, y, w, h);
        tipVisible = false;
    };

    ~Clickable()
    {
        delete tx;
        delete ft;
    };

    void showToolTip()
    {
        tipVisible = true;
    };

    void draw(sf::RenderWindow& win)
    {
        win.draw(sp);
        win.draw(t);
        if(tipVisible)
        {
            sf::RectangleShape background;
            background.setSize(sf::Vector2f(t.getGlobalBounds().width, t.getGlobalBounds().height+20));
            background.setPosition(t.getPosition().x, bounds.getY());
            background.setFillColor(sf::Color::Black);
            win.draw(background);
            win.draw(t);
            //std::cout << "recycle" << std::endl;
        }
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
            t.setPosition(bounds.getX()+bounds.getWidth(), bounds.getY());
            return true;
        }
        else
        { 
            sp.setTextureRect(sf::IntRect(0, 0, 64, 64));
            tipVisible = false;
        }
        return false;
    };

};