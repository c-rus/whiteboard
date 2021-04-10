#pragma once
#include <list>
#include "squiggle.h"

class Board
{
private:
    std::list<Squiggle*> scribs;
    std::list<Squiggle*> rScribs;

public:
    Board();
    ~Board();

    void startSqui(sf::Vector2i& loc, int w, sf::Color i);
    void continueSqui(sf::Vector2i& loc, int w, sf::Color i);

    void draw(sf::RenderWindow& win);

    void pan(sf::Vector2i& offset);

    void redo();
    void undo();
    void clear();
};