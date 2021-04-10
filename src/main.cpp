#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "squiggle.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Whiteboard", sf::Style::None);
    std::cout << "Welcome to whiteboard!" << std::endl;
    window.setFramerateLimit(60);
    
    std::list<Squiggle*> artwork;
    sf::Vector2i cord, lastCord, diff;

    sf::Color penInk = sf::Color::Black;
    int penSize = 6;

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
                pressed = false;
                pan = false;
            }
            else if(e.type == sf::Event::MouseButtonPressed)
            {
                pressed = true;
                
                cord = sf::Mouse::getPosition(window);
                lastCord = cord;
                diff = sf::Vector2i(0,0);
                //std::cout << cord.x << " " << cord.y << std::endl;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    pan = true;
                else
                    artwork.push_back(new Squiggle(cord, penSize, penInk));
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
                        if(artwork.size() > 0)
                        {
                            delete artwork.back();
                            artwork.pop_back();
                            std::cout << "undo squiggle!" << std::endl;
                        }
                    }
                }
                else if(e.key.code == sf::Keyboard::G)
                {
                    penInk = sf::Color::Green;
                }
                else if(e.key.code == sf::Keyboard::B)
                {
                    penInk = sf::Color::Black;
                }
                else if(e.key.code == sf::Keyboard::R)
                {
                    penInk = sf::Color::Red;
                }
                else if(e.key.code == sf::Keyboard::L)
                {
                    penInk = sf::Color::Blue;
                }
                else if(e.key.code == sf::Keyboard::Up)
                {
                    penSize+=1;
                }
                else if(e.key.code == sf::Keyboard::Down)
                {
                    penSize-=1;
                    if(penSize < 1)
                        penSize = 1;
                }
                else if(e.key.code == sf::Keyboard::E)
                {
                    eraser = !eraser;
                }
                else if(e.key.code == sf::Keyboard::C)
                {
                    while(artwork.size() > 0)
                    {
                        Squiggle* s = artwork.back();
                        delete s;
                        artwork.pop_back();
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

        if(pressed && !eraser && !pan)
        {
            cord = sf::Mouse::getPosition(window);
            artwork.back()->addPoint(cord, penSize, penInk);
        }
        else if(pressed && eraser && !pan)
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
            for(auto it = artwork.begin(); it != artwork.end(); it++)
            {
                (*it)->move(diff);
            }
            lastCord = cord;
        }

        window.clear(sf::Color::White);

        for(auto it = artwork.begin(); it != artwork.end(); it++)
        {
            (*it)->draw(window);
        }
        window.display();
    }

    while(artwork.size() > 0)
    {
        Squiggle* s = artwork.back();
        delete s;
        artwork.pop_back();
    }
}