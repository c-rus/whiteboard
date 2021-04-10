#include "squiggle.h"

Squiggle::Squiggle(sf::Vector2i& start, int width, sf::Color color)
{
    points.push(start);
    mag = 1;
    this->width = width;
    sf::RectangleShape* l = new sf::RectangleShape(sf::Vector2f(width, width));
    l->setFillColor(color);
    l->setPosition(sf::Vector2f(start));
    lines.push_back(l);
}

void Squiggle::draw(sf::RenderWindow& win)
{
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        win.draw(**it);
    }
}

//TODO: Work on fixing zoom 
void Squiggle::zoom(int magnify, sf::Vector2i& mPoint)
{
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        auto& l = *it;
        //l->setScale(mag, mag);
        sf::Vector2i nextStep = sf::Vector2i(magnify, magnify);
        if(l->getPosition().x > mPoint.x)
        {
            nextStep.x = -nextStep.x;
        }
        else if(l->getPosition().x == mPoint.x)
        {
            nextStep.x = 0;
        }
        if(l->getPosition().y > mPoint.y)
        {
            nextStep.y = -nextStep.y;
        }
        else if(l->getPosition().y == mPoint.y)
        {
            nextStep.y = 0;
        }
        
        l->setPosition(l->getPosition()+sf::Vector2f(nextStep));
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

void Squiggle::addPoint(sf::Vector2i& p, int width, sf::Color color)
{
    sf::Vector2f prev = sf::Vector2f(points.top());
    if(prev == sf::Vector2f(p))
        return;
    //calculate slopes
    float slopeYoverX = 0;
    float slopeXoverY = 0;
    if(prev.x > p.x)
        slopeYoverX = float(float(p.y-prev.y)/(float(prev.x-p.x)));
    else if(prev.x < p.x)
        slopeYoverX = float(float(p.y-prev.y)/(float(p.x-prev.x)));

    if(prev.y > p.y)
        slopeXoverY = float(float(p.x-prev.x)/(float(prev.y-p.y)));
    else if(prev.y < p.y)
        slopeXoverY = float(float(p.x-prev.x)/(float(p.y-prev.y)));

    //determine the easier slope to work with
    bool xAxis = !(abs(int(slopeYoverX)) > abs(int(slopeXoverY)));

    //choose x-axis as independent
    if(prev.x != p.x && (xAxis || prev.y == p.y))
    {
        while(prev.x != p.x)
        {
            prev.x = (prev.x < p.x) ? prev.x+1 : prev.x-1;
            prev.y+=slopeYoverX;

            sf::RectangleShape* l = new sf::RectangleShape(sf::Vector2f(width, width));
            l->setFillColor(color);
            l->setPosition(sf::Vector2f(prev));
            lines.push_back(l);
        }
    }
    //choose y-axis as independent
    else
    {
        while(prev.y != p.y)
        {
            prev.y = (prev.y < p.y) ? prev.y+1 : prev.y-1;
            prev.x+=slopeXoverY;

            sf::RectangleShape* l = new sf::RectangleShape(sf::Vector2f(width, width));
            l->setFillColor(color);
            l->setPosition(sf::Vector2f(prev));
            lines.push_back(l);
        }
    }
    points.push(p);
}

int Squiggle::count()
{
    return lines.size();
}