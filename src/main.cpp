#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "squiggle.h"
#include "stylus.h"
#include "board.h"

/*
TODO: key outlooks
    - draw only top-most pixel (use unordered_map or array)
    - draw only visible pixels (not offscreen pixels)
*/
int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Whiteboard", sf::Style::None);
    std::cout << "Welcome to whiteboard!" << std::endl;
    window.setFramerateLimit(60);
    
    sf::Vector2i loc, prevLoc, diff;

    Stylus pen(5, sf::Color(0,0,0));
    Board b;

    int magnifier = 10;

    bool pressed, pan, eraser, cmd;
    pressed = pan = eraser = cmd = false;
    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
            else if(e.type == sf::Event::MouseButtonReleased)
            {
                pressed = pan = false;
            }
            else if(e.type == sf::Event::MouseButtonPressed)
            {   
                loc = sf::Mouse::getPosition(window);
                prevLoc = loc;
                diff = sf::Vector2i(0,0);
                //std::cout << cord.x << " " << cord.y << std::endl;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    pan = true;
                else if(!eraser)
                {
                    b.startSqui(loc, pen.getWidth(), pen.getInk());
                    pressed = true;
                }
            }
            else if(e.type == sf::Event::MouseWheelMoved)
            {
                std::cout << e.mouseWheel.delta << std::endl;
                loc = sf::Mouse::getPosition(window);
                magnifier = e.mouseWheel.delta;
            }
            else if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::LSystem)
                {
                    cmd = true;
                }
                else if(e.key.code == sf::Keyboard::Z)
                {
                    if(cmd) //undo
                        b.undo();
                }
                else if(e.key.code == sf::Keyboard::X)
                {
                    if(cmd) //redo
                        b.redo();
                }
                else if(e.key.code == sf::Keyboard::W)
                {

                }
                else if(e.key.code == sf::Keyboard::G)
                {
                    pen.setInk(sf::Color(34,139,34));
                }
                else if(e.key.code == sf::Keyboard::B)
                {
                    pen.setInk(sf::Color::Black);
                }
                else if(e.key.code == sf::Keyboard::O)
                {
                    pen.setInk(sf::Color(255,125,0));
                }
                else if(e.key.code == sf::Keyboard::R)
                {
                    pen.setInk(sf::Color::Red);
                }
                else if(e.key.code == sf::Keyboard::L)
                {
                    pen.setInk(sf::Color::Blue);
                }
                else if(e.key.code == sf::Keyboard::Up)
                {
                    pen.setWidth(pen.getWidth()+1);
                }
                else if(e.key.code == sf::Keyboard::Down)
                {
                    pen.setWidth(pen.getWidth()-1);
                }
                else if(e.key.code == sf::Keyboard::E)
                {
                    eraser = !eraser;
                }
                else if(e.key.code == sf::Keyboard::C)
                {
                    b.clear();
                }
            }
            else if(e.type == sf::Event::KeyReleased)
            {
                if(e.key.code == sf::Keyboard::LSystem)
                {
                    cmd = false;
                }
            }
        }

        loc = sf::Mouse::getPosition(window);
        if(pressed && !eraser)
            b.continueSqui(loc, pen.getWidth(), pen.getInk());
        else if(pressed)
        {
            //TODO: Fix up erasing
            int eRange = 6;
            for(int i = -eRange; i <= eRange; i++)
            {
                sf::Vector2f tempC = sf::Vector2f(loc);
                tempC.x+=i;
                for(int j = -eRange; j <= eRange; j++)
                {
                    tempC.y+=j;
                    std::string cordKey = std::to_string(int(tempC.x)) + "," + std::to_string(int(tempC.y));
                }
            }
        }
        else if(pan)
        {
            diff = loc - prevLoc;
            b.pan(diff);
            prevLoc = loc;
        }

        window.clear(sf::Color::White);
        b.draw(window);
        window.display();
    }
}