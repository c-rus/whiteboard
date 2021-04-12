#include "squiggle.h"

Squiggle::Squiggle(sf::Vector2i& start, int width, sf::Color color)
{
    this->prev = sf::Vector2f(start);
    sf::CircleShape* l = new sf::CircleShape(width);
    l->setFillColor(color);
    l->setPosition(sf::Vector2f(start));
    lines.push_back(l);
    bounds = Box(start.x, start.y, width, width);
}

Squiggle::~Squiggle()
{
    while(!lines.empty())
    {
        delete lines.back();
        lines.pop_back();
    }
}

void Squiggle::draw(sf::RenderWindow& win)
{
    for(auto it = lines.begin(); it != lines.end(); it++)
        win.draw(**it);
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
    bounds.shift(offset.x, offset.y);
}

bool Squiggle::addPoint(sf::Vector2i& p, int w, sf::Color color)
{
    if(prev == sf::Vector2f(p))
        return false;
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

            sf::CircleShape* l = new sf::CircleShape(w);
            l->setFillColor(color);
            l->setPosition(sf::Vector2f(prev));
            lines.push_back(l);
            bounds.stretch(prev.x+w, prev.y+w);
        }
    }
    //choose y-axis as independent
    else
    {
        while(prev.y != p.y)
        {
            prev.y = (prev.y < p.y) ? prev.y+1 : prev.y-1;
            prev.x+=slopeXoverY;

            sf::CircleShape* l = new sf::CircleShape(w);
            l->setFillColor(color);
            l->setPosition(sf::Vector2f(prev));
            lines.push_back(l);
            bounds.stretch(prev.x+w, prev.y+w);
        }
    }
    prev = sf::Vector2f(p);
    return true;
}

int Squiggle::count()
{
    return lines.size();
}

Box& Squiggle::getBounds()
{
    return bounds;
}

Squiggle::Squiggle(std::fstream& file)
{
    int size = 0;
    file.read((char*)&size, sizeof(size));

    //what were the bounds?
    int x = 0;
    file.read((char*)&x, sizeof(x));
    int y = 0;
    file.read((char*)&y, sizeof(y));
    int w = -1;
    file.read((char*)&w, sizeof(w));
    int h = -1;
    file.read((char*)&h, sizeof(h));
    bounds = Box(x, y, w, h);
    
    for(int i = 0; i < size; i++)
    {
        sf::CircleShape* l = new sf::CircleShape();
        //location
        file.read((char*)&x, sizeof(x));
        file.read((char*)&y, sizeof(y));
        //color
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char a = 0;
        file.read((char*)&r, sizeof(r));
        file.read((char*)&g, sizeof(g));
        file.read((char*)&b, sizeof(b));
        file.read((char*)&a, sizeof(a));
        //width
        unsigned short width = 0;
        file.read((char*)&width, sizeof(width));

        l->setPosition(sf::Vector2f(x, y));
        l->setFillColor(sf::Color(r, g, b, a));
        l->setRadius(width);
        lines.push_back(l);
    }
}

void Squiggle::save(std::fstream& file)
{
    int size = lines.size();
    file.write((char*)&size, sizeof(size)); //to remember how many points are in this squiggle

    //save the bounds
    int temp = bounds.getX();
    file.write((char*)&temp, sizeof(temp));
    temp = bounds.getY();
    file.write((char*)&temp, sizeof(temp));
    temp = bounds.getWidth();
    file.write((char*)&temp, sizeof(temp));
    temp = bounds.getHeight();
    file.write((char*)&temp, sizeof(temp));

    //save every pixel
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        sf::CircleShape* l = *it;
        //location
        int x = l->getPosition().x;
        int y = l->getPosition().y;;
        file.write((char*)&x, sizeof(x));
        file.write((char*)&y, sizeof(y));
        //color
        unsigned char r = l->getFillColor().r;
        unsigned char g = l->getFillColor().g;
        unsigned char b = l->getFillColor().b;
        unsigned char a = l->getFillColor().a;
        file.write((char*)&r, sizeof(r));
        file.write((char*)&g, sizeof(g));
        file.write((char*)&b, sizeof(b));
        file.write((char*)&a, sizeof(a));
        //width
        unsigned short w = (unsigned short)l->getRadius();
        file.write((char*)&w, sizeof(w));
    }
}