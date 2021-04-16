#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"

struct Color
{
private:
    
public:
    enum STD
    {
        Black = 255,
        Gray = 471604479, //168430335,
        Red = 4278190335,
        White = 4294967295,
        Green = 16711935,
        GreenAlt = 583541503,
        Blue = 65535,
        Orange = 4289003775,
        Transparent = 0,
    };

private:
    std::string name;
    std::string hexVal;
    unsigned int intVal;
    unsigned char R = 0;
    unsigned char G = 0;
    unsigned char B = 0;
    unsigned char A = 0;
    //alter order to change how bits are formatted in the hex codes
    unsigned char* format[4] = {&R, &G, &B, &A};

    sf::Color dye;
    static unsigned char GetHexDigit(unsigned char c);

public:
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
    Color(unsigned int val);
    Color(sf::Color& c);
    Color();
    //Given an integer value, convert it to RGB and Hex values
    void SetValue(unsigned int v);
    void SetName(const char* n);

    bool operator==(const Color& rhs);
    void operator()(unsigned int v);

    //Accessors
    unsigned char GetR() const;
    unsigned char GetG() const;
    unsigned char GetB() const;
    unsigned char GetA() const;
    std::string GetHexValue() const;
    unsigned int getIntValue() const;
    std::string GetName() const;

    sf::Color& getSFColor();
};