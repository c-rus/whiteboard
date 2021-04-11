#include "hud.h"

HUD::HUD(int w, int h)
{
    knobs.push_back(Clickable(10, 10, 64, 64));
    inspecting = knobs.end();
    //knobs.push_back(Clickable(w-10-64, 10, 64, 64));
}

void HUD::resize(int w, int h)
{
    //implement something here to ensure HUD stays fully visible in workspace
}

bool HUD::interact(sf::Vector2i& mLoc, Board& b)
{
    Box mouseBounds(mLoc.x, mLoc.y, 0, 0);
    for(auto it = knobs.begin(); it != knobs.end(); it++)
    {
        if(((*it).getBounds().contains(mouseBounds)))
        {
            std::cout << "reset canvas!" << std::endl;
            b.clear();
            return true;
        }
    }
    
    return false;
}

bool HUD::inspect(sf::Vector2i& mLoc)
{
    auto prevInspect = inspecting;
    inspecting = knobs.end();
    for(auto it = knobs.begin(); it != knobs.end(); it++)
    {
        if((*it).highlight(mLoc))
            inspecting = it;
    }
    
    return (prevInspect != inspecting);
}

void HUD::draw(sf::RenderTexture& surf)
{
    surf.clear(sf::Color::Transparent);
    for(auto it = knobs.begin(); it != knobs.end(); it++)
        (*it).draw(surf);
}

sf::Sprite HUD::getLayer(sf::RenderTexture& surf)
{
    surf.display();
    return sf::Sprite(surf.getTexture());
}