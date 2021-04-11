#pragma once
#include <iostream>

class Box
{
private:
    int width, height, x, y;

public:
    Box(int x=0, int y=0, int w=-1, int h=-1)
    {
        width = w;
        height = h;
        this->x = x;
        this->y = y;
    };

    int getWidth() { return width; };

    int getHeight() { return height; };

    int getX() { return x; };

    int getY() { return y; };

    void shift(int a, int b)
    {
        x+=a;
        y+=b;
    }

    void print()
    {
        std::cout << "x: " << x << " y: " << y;
        std::cout << "\t width: " << width << " height:" << height << std::endl; 
    }

    void stretch(int a, int b)
    {
        if(a < x)
        {
            int diff = x-a;
            x = a;
            width = width+diff;
        }  
        else if(a > x+width)
        {
            width = a-x;
        }

        if(b < y)
        {
            int diff = y-b;
            y = b;
            height = height+diff;
        }
        else if(b > y+height)
        {
            height = b-y;
        }
    }

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