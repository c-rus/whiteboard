#include "hud.h"

HUD::HUD(int w, int h)
{
    //knobs.push_back(Clickable(10, 10, 64, 64));
    inspecting = knobs.end();
    counter = 0;
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

void HUD::update()
{
    if(inspecting != knobs.end())
    {
        counter++;
        if(counter >= delay)
        {
            if(counter == delay)
                (*inspecting).showToolTip();
        }
    }
    else
        counter = 0;  
}

void HUD::inspect(sf::Vector2i& mLoc)
{
    auto prevInspect = inspecting;
    inspecting = knobs.end();
    for(auto it = knobs.begin(); it != knobs.end(); it++)
    {
        if((*it).highlight(mLoc))
            inspecting = it;
    }
}

void HUD::draw(sf::RenderWindow& win)
{
    for(auto it = knobs.begin(); it != knobs.end(); it++)
        (*it).draw(win);
}