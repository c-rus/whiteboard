#include "color.h"

void Color::SetValue(unsigned int v)
{
    intVal = v;

    *format[3] = v & 255;
    *format[2] =  (v >> 8) & 255;
    *format[1] = (v >> 16) & 255;
    *format[0] = (v >> 24) & 255;

    hexVal = "0x00000000";
    //iterate through each color value to assign two hexadecimal values per each
    for (int i = 0; i < 4; i++)
    {
        unsigned char guy1 = *format[i] / 16;
        hexVal[2+(i*2)] = GetHexDigit(guy1) + 0;

        unsigned char guy2 = *format[i] % 16;
        hexVal[3+(i*2)] = GetHexDigit(guy2) + 0;
    }

    dye = sf::Color(R, G, B, A);
}

Color::Color(unsigned int val)
{
    SetValue(val);
}

Color::Color() : Color(0, 0, 0, 0)
{

};

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    R = r;
    G = g;
    B = b;
    A = a;

    intVal = *format[3] & 255;
    intVal = intVal << 8;
    intVal |= *format[2] & 255;
    intVal = intVal << 8;
    intVal |= *format[1] & 255;
    intVal = intVal << 8;
    intVal |= *format[0] & 255;

    hexVal = "0x00000000";
    //iterate through each color value to assign two hexadecimal values per each
    for (int i = 0; i < 4; i++)
    {
        unsigned char guy1 = *format[i] / 16;
        hexVal[2+(i*2)] = GetHexDigit(guy1) + 0;

        unsigned char guy2 = *format[i] % 16;
        hexVal[3+(i*2)] = GetHexDigit(guy2) + 0;
    }
    
    dye = sf::Color(R, G, B, A);
}

bool Color::operator==(const Color& rhs)
{
    return (intVal == rhs.intVal && A == rhs.A);
}

void Color::operator()(unsigned int v)
{
    *this = Color(v);
}

unsigned char Color::GetHexDigit(unsigned char c)
{
    switch (c) {
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
        default:
            //numbers on the ASCII table begin at 48
            return ((unsigned char)int(c) + 48);
    }
}

void Color::SetName(const char* n)
{
    this->name = n;
}

//*=====Accessors=====*//

unsigned char Color::GetR() const
{
    return R;
}

unsigned char Color::GetG() const
{
    return G;
}

unsigned char Color::GetB() const
{
    return B;
}

unsigned char Color::GetA() const
{
    return A;
}

std::string Color::GetHexValue() const
{
    return hexVal;
}

std::string Color::GetName() const
{
    return name;
}

sf::Color& Color::getSFColor()
{
    return dye;
}