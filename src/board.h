#pragma once
#include <list>
#include "squiggle.h"
#include <iostream>
#include <unordered_map>

class Board
{
private:
    std::list<Squiggle*> scribs;
    std::list<Squiggle*> rScribs;

    int width;
    int height;

    const int MAX_W = sf::VideoMode::getDesktopMode().width;
    const int MAX_H = sf::VideoMode::getDesktopMode().height;

public:
    Board(int w, int h);
    ~Board();

    void startSqui(sf::Vector2i& loc, int w, sf::Color i);
    void continueSqui(sf::Vector2i& loc, int w, sf::Color i);

    void draw(sf::RenderWindow& win);

    void pan(sf::Vector2i& offset);

    void erase(sf::Vector2i& loc, int w);

    void resize(int w, int h);
    void redo();
    void undo();
    void clear();
};