#include "squiggle.h"

Squiggle::Squiggle(sf::Vector2i& start, int width, Color c)
{
    this->prev = sf::Vector2f(start);
    lines.push_back(new Pixel(prev, width, c));
    bounds = Box(start.x, start.y, width*2, width*2);
    optimized = false;
}

Squiggle::~Squiggle()
{
    while(!lines.empty())
    {
        delete lines.back();
        lines.pop_back();
    }
    delete rt;
    delete sp;
}

void Squiggle::draw(sf::RenderWindow& win)
{
    if(optimized)
        win.draw(*sp);
    else
    {
        for(auto it = lines.begin(); it != lines.end(); it++)
            win.draw((*it)->getDot());
    }
}

//TODO: Work on fixing zoom 
void Squiggle::zoom(int magnify, sf::Vector2i& mPoint)
{
    
}

void Squiggle::move(sf::Vector2i& offset)
{
    if(optimized)
        sp->setPosition(sp->getPosition()+sf::Vector2f(offset));
    else
    {
        for(auto it = lines.begin(); it != lines.end(); it++)
        {
            auto& l = (*it)->getDot();
            l.setPosition(l.getPosition()+sf::Vector2f(offset));
        }
    }
    bounds.shift(offset.x, offset.y);
}

void Squiggle::compress()
{
    rt = new sf::RenderTexture();
    rt->create(bounds.getWidth(), bounds.getHeight());
    rt->clear(Color(0,0,0,0).getSFColor());
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        auto& d = (*it)->getDot();
        d.setPosition(d.getPosition()-sf::Vector2f(bounds.getX(), bounds.getY()));
        rt->draw(d);
    }
    rt->display();
    sp = new sf::Sprite();
    sp->setTexture(rt->getTexture());
    sp->setPosition(bounds.getX(), bounds.getY());
    optimized = true;
}

bool Squiggle::addPoint(sf::Vector2i& p, int w, Color c)
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
            bool headedRight = (prev.x < p.x);
            bool headedDown =  (prev.y < p.y);

            prev.x = (prev.x < p.x) ? prev.x+1 : prev.x-1;
            prev.y += slopeYoverX;

            lines.push_back(new Pixel(prev, w, c));
            int borderX = (headedRight) ? prev.x+(2*w) : prev.x;
            int borderY = (headedDown) ? prev.y+(2*w) : prev.y;
            bounds.stretch(borderX, borderY);
        }
    }
    //choose y-axis as independent
    else
    {
        while(prev.y != p.y)
        {
            bool headedDown = (prev.y < p.y);
            bool headedRight = (prev.x < p.x);

            prev.y = (prev.y < p.y) ? prev.y+1 : prev.y-1;
            prev.x += slopeXoverY;

            lines.push_back(new Pixel(prev, w, c));
            int borderY = (headedDown) ? prev.y+(2*w) : prev.y;
            int borderX = (headedRight) ? prev.x+(2*w) : prev.x;
            bounds.stretch(borderX, borderY);
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
        Pixel* p = new Pixel();
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

        p->getDot().setPosition(sf::Vector2f(x, y));
        p->getDot().setFillColor(sf::Color(r, g, b, a));
        p->getDot().setRadius(width);
        lines.push_back(p);
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
        auto l = (*it)->getDot();
        //location
        int x = l.getPosition().x;
        int y = l.getPosition().y;;
        file.write((char*)&x, sizeof(x));
        file.write((char*)&y, sizeof(y));
        //color
        unsigned char r = l.getFillColor().r;
        unsigned char g = l.getFillColor().g;
        unsigned char b = l.getFillColor().b;
        unsigned char a = l.getFillColor().a;
        file.write((char*)&r, sizeof(r));
        file.write((char*)&g, sizeof(g));
        file.write((char*)&b, sizeof(b));
        file.write((char*)&a, sizeof(a));
        //width
        unsigned short w = (unsigned short)l.getRadius();
        file.write((char*)&w, sizeof(w));
    }
}