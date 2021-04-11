#pragma once
#include "SFML/Graphics.hpp"
#include "clickable.h"
#include "board.h"

class HUD
{
private:
    std::list<Clickable> knobs;
    std::list<Clickable>::iterator inspecting;

public:
    HUD(int w, int h);
    void draw(sf::RenderTexture& surf);
    
    sf::Sprite getLayer(sf::RenderTexture& surf);
    void resize(int w, int h);
    bool inspect(sf::Vector2i& mLoc);
    bool interact(sf::Vector2i& mLoc, Board& b);
};