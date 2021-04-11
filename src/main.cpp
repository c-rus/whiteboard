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
    -everytime things are moved, try to fit into board's array (start with top squiggles)
*/
int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Whiteboard");
    std::cout << "Welcome to whiteboard!" << std::endl;
    window.setFramerateLimit(60);
    sf::Vector2i loc, prevLoc, diff;

    Stylus pen(5, sf::Color(0,0,0));
    Board b(window.getSize().x, window.getSize().y);

    bool pressed, pan, cmd;
    pressed = pan = cmd = false;
    while(window.isOpen())
    {
        b.deassertRefresh();

        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
            else if(e.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, e.size.width, e.size.height);
                b.resize(visibleArea.width, visibleArea.height);
                window.setView(sf::View(visibleArea));
            }
            else if(e.type == sf::Event::MouseButtonReleased)
            {
                pressed = pan = false;
            }
            else if(e.type == sf::Event::MouseButtonPressed)
            {   
                pressed = true;
                loc = sf::Mouse::getPosition(window);
                prevLoc = loc;
                diff = sf::Vector2i(0,0);
                //std::cout << cord.x << " " << cord.y << std::endl;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    pan = true;
                else if(pen.getMode() == Stylus::DRAW)
                    b.startSqui(loc, pen.getWidth(), pen.getInk());
            }
            else if(e.type == sf::Event::MouseWheelMoved)
            {
                //std::cout << e.mouseWheel.delta << std::endl;
                loc = sf::Mouse::getPosition(window);
                sf::Vector2i scrollPace(0, e.mouseWheel.delta*4);
                b.pan(scrollPace);
            }
            else if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::LSystem) cmd = true;
                else if(e.key.code == sf::Keyboard::Z && cmd) b.undo();
                else if(e.key.code == sf::Keyboard::X && cmd) b.redo();
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
                    pen.swapMode(Stylus::ERASE);
                }
                else if(e.key.code == sf::Keyboard::D)
                {
                    pen.swapMode(Stylus::DRAW);
                }
                else if(e.key.code == sf::Keyboard::C) b.clear();
            }
            else if(e.type == sf::Event::KeyReleased)
            {
                if(e.key.code == sf::Keyboard::LSystem) cmd = false;
            }
        }

        loc = sf::Mouse::getPosition(window);
        if(pan)
        {
            diff = loc - prevLoc;
            b.pan(diff);
            prevLoc = loc;
        }
        else if(pressed)
        {
            switch(pen.getMode())
            {
                case Stylus::DRAW:
                    b.continueSqui(loc, pen.getWidth(), pen.getInk());
                    break;
                case Stylus::ERASE:
                    b.erase(loc, pen.getWidth());
                    break;
                default:
                    std::cout << "ERROR: Unknown mode." << std::endl;
            }
        }

        b.draw(window);
        window.display();
    }
}