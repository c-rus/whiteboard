#pragma once
#include <iostream>
#include "color.h"

class Box
{
private:
    int width, height, x, y;
    sf::RectangleShape outline;
    bool visible;

public:
    Box(int x=0, int y=0, int w=-1, int h=-1)
    {
        width = w;
        height = h;
        this->x = x;
        this->y = y;
        outline.setSize(sf::Vector2f(width, height));
        outline.setFillColor(Color(Color::Transparent).getSFColor());
        outline.setOutlineColor(Color(Color::Black).getSFColor());
        outline.setOutlineThickness(2);
        outline.setPosition(x, y);
        visible = false;
    };

    sf::RectangleShape& getOutline()
    {
        return outline;
    };

    void draw(sf::RenderWindow& win)
    {
        if(visible)
            win.draw(outline);
    };

    void setVisibility(bool b)
    {
        visible = b;
    };

    int getWidth() { return width; };

    int getHeight() { return height; };

    int getX() { return x; };

    int getY() { return y; };

    void adjust(int w, int h)
    {
        outline.setSize(sf::Vector2f(w, h));

        if(w < 0)
        {
            x = outline.getPosition().x+w;
            width = -w;
        }
        else
        {
            x = outline.getPosition().x;
            width = w;
        }

        if(h < 0)
        {
            y = outline.getPosition().y+h;
            height = -h;
        }
        else
        {
            y = outline.getPosition().y;
            height = h;
        }
    };

    void shift(int a, int b)
    {
        x+=a;
        y+=b;
        outline.setPosition(x, y);
    };

    bool isVisible()
    {
        return visible;
    };

    void print()
    {
        std::cout << "x: " << x << " y: " << y;
        std::cout << "\t width: " << width << " height:" << height << std::endl; 
    };

    bool stretch(int a, int b)
    {
        bool triggered = false;
        if(a < x)
        {
            int diff = x-a;
            x = a;
            width = width+diff;
            triggered = true;
        }  
        else if(a > x+width)
        {
            width = a-x;
            triggered = true;
        }

        if(b < y)
        {
            int diff = y-b;
            y = b;
            height = height+diff;
            triggered = true;
        }
        else if(b > y+height)
        {
            height = b-y;
            triggered = true;
        }

        if(triggered)
        {
            outline.setPosition(x, y);
            outline.setSize(sf::Vector2f(width, height));
        }

        return triggered;
    };

    bool contains(Box& rhs)
    {
        return (((rhs.x >= x && rhs.x <= x+width) || (rhs.x+rhs.width >= x && rhs.x+rhs.width <= x+width)) &&
                ((rhs.y >= y && rhs.y <= y+height) || (rhs.y+rhs.height >= y && rhs.y+rhs.height <= y+height)));
    };

    bool intersect(Box& rhs)
    {
        return (contains(rhs) ||
                (x >= rhs.x && x+width <= rhs.x+rhs.width && ((rhs.y >= y && rhs.y <= y+height) || 
                                                              (rhs.y+rhs.height >= y && rhs.y+rhs.height <= y+height))) ||
                (y >= rhs.y && y+height <= rhs.y+rhs.height && ((rhs.x >= x && rhs.x <= x+width) || 
                                                              (rhs.x+rhs.width >= x && rhs.x+rhs.width <= x+width)))                                              
                );
    };
};