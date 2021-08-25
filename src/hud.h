#pragma once
#include "SFML/Graphics.hpp"
#include "clickable.h"
#include "board.h"

class HUD
{
private:
    std::list<Clickable> knobs;
    std::list<Clickable>::iterator inspecting;
    int counter;
    int delay = 80;

public:
    HUD(int w, int h);
    void update();
    void draw(sf::RenderWindow& win);

    void resize(int w, int h);
    void inspect(sf::Vector2i& mLoc);
    bool interact(sf::Vector2i& mLoc, Board& b);
};