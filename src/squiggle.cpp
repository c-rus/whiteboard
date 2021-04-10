#include "squiggle.h"

Squiggle::Squiggle(sf::Vector2i& start, int size, sf::Color color)
{
    points.push(start);
    sf::RectangleShape* l = new sf::RectangleShape();
    l->setFillColor(color);
    l->setPosition(sf::Vector2f(start));
    l->setSize(sf::Vector2f(size, size));
    lines.push_back(l);
}

void Squiggle::draw(sf::RenderWindow& win)
{
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        win.draw(**it);
    }
}

void Squiggle::move(sf::Vector2i& offset)
{
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        auto& l = *it;
        l->setPosition(l->getPosition()+sf::Vector2f(offset));
    }
}

void Squiggle::addPoint(sf::Vector2i& p, int size, sf::Color color)
{
    sf::Vector2f lastPoint = sf::Vector2f(points.top());
    if(lastPoint == sf::Vector2f(p))
        return;
    //TODO: Generate smoother lines are steeper slopes
    if(abs((int(p.x-lastPoint.x))) != 0 && abs((int(float(p.y-lastPoint.y)/(float(p.x-lastPoint.x))))) < 7)
    {
        float slope = float(float(p.y-lastPoint.y)/(float(p.x-lastPoint.x)));
        if(lastPoint.x > p.x)
            slope = float(float(p.y-lastPoint.y)/(float(lastPoint.x-p.x)));
        while(lastPoint.x != p.x)
        {
            if(lastPoint.x < p.x)
                lastPoint.x++;
            else
                lastPoint.x--;

            lastPoint.y+=slope;
            sf::RectangleShape* l = new sf::RectangleShape();
            l->setFillColor(color);
            l->setSize(sf::Vector2f(size, size));
            l->setPosition(sf::Vector2f(lastPoint));
            lines.push_back(l);
        }
    }
    //undefined slope
    else
    {
        float slope = float(float(p.x-lastPoint.x)/(float(p.y-lastPoint.y)));
        if(lastPoint.y > p.y)
            slope = float(float(p.x-lastPoint.x)/(float(lastPoint.y-p.y)));
        if(p.x == lastPoint.x)
            slope = 0;

        while(lastPoint.y != p.y)
        {
            if(lastPoint.y < p.y)
                lastPoint.y++;
            else
                lastPoint.y--;

            lastPoint.x+=slope;

            sf::RectangleShape* l = new sf::RectangleShape();
            l->setFillColor(color);
            l->setSize(sf::Vector2f(size, size));
            l->setPosition(sf::Vector2f(lastPoint));
            lines.push_back(l);
        }
    }
    points.push(p);
}