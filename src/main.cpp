#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "squiggle.h"
#include "stylus.h"

/*
TODO: key outlooks
    - draw only top-most pixel (use unordered_map)
    - draw only visible pixels (not offscreen pixels)
*/
int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Whiteboard", sf::Style::None);
    std::cout << "Welcome to whiteboard!" << std::endl;
    window.setFramerateLimit(60);
    
    std::list<Squiggle*> doodle;
    std::list<Squiggle*> reverseDoodle;
    sf::Vector2i cord, lastCord, diff;

    Stylus pen(5, sf::Color(0,0,0));

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
                cord = sf::Mouse::getPosition(window);
                lastCord = cord;
                diff = sf::Vector2i(0,0);
                //std::cout << cord.x << " " << cord.y << std::endl;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    pan = true;
                else if(!eraser)
                {
                    doodle.push_back(new Squiggle(cord, pen.getWidth(), pen.getInk()));
                    pressed = true;
                }
            }
            else if(e.type == sf::Event::MouseWheelMoved)
            {
                std::cout << e.mouseWheel.delta << std::endl;
                cord = sf::Mouse::getPosition(window);
                magnifier = e.mouseWheel.delta;
                if(magnifier != 0)
                {
                    for(auto it = doodle.begin(); it != doodle.end(); it++)
                    {
                        (*it)->zoom(magnifier, cord);
                    }
                }
            }
            else if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::LSystem)
                {
                    cmd = true;
                }
                else if(e.key.code == sf::Keyboard::Z)
                {
                    if(cmd)
                    {
                        if(doodle.size() > 0)
                        {
                            reverseDoodle.push_back(doodle.back());
                            doodle.pop_back();
                            std::cout << "undo squiggle!" << std::endl;
                        }
                    }
                }
                else if(e.key.code == sf::Keyboard::X)
                {
                    if(cmd)
                    {
                        if(reverseDoodle.size() > 0)
                        {
                            doodle.push_back(reverseDoodle.back());
                            reverseDoodle.pop_back();
                            std::cout << "redo squiggle!" << std::endl;
                        }
                    }
                }
                else if(e.key.code == sf::Keyboard::W)
                {
                    int totalPoints = 0;
                    for(auto it = doodle.begin(); it != doodle.end(); it++)
                    {
                        totalPoints+=(*it)->count();
                    }
                    std::cout << "total dots on board: " << totalPoints << std::endl;
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
                    while(doodle.size() > 0)
                    {
                        Squiggle* s = doodle.back();
                        delete s;
                        doodle.pop_back();
                    }
                    while(reverseDoodle.size() > 0)
                    {
                        Squiggle* s = reverseDoodle.back();
                        delete s;
                        reverseDoodle.pop_back();
                    }
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

        if(pressed && !eraser)
        {
            cord = sf::Mouse::getPosition(window);
            doodle.back()->addPoint(cord, pen.getWidth(), pen.getInk());
        }
        else if(pressed)
        {
            //TODO: Fix up erasing
            int eRange = 6;
            for(int i = -eRange; i <= eRange; i++)
            {
                sf::Vector2f tempC = sf::Vector2f(cord);
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
            cord = sf::Mouse::getPosition(window);
            diff = cord - lastCord;
            for(auto it = doodle.begin(); it != doodle.end(); it++)
                (*it)->move(diff);
            lastCord = cord;
        }

        window.clear(sf::Color::White);

        for(auto it = doodle.begin(); it != doodle.end(); it++)
        {
            (*it)->draw(window);
        }
        window.display();
    }

    while(doodle.size() > 0)
    {
        Squiggle* s = doodle.back();
        delete s;
        doodle.pop_back();
    }
    while(reverseDoodle.size() > 0)
    {
        Squiggle* s = reverseDoodle.back();
        delete s;
        reverseDoodle.pop_back();
    }
}