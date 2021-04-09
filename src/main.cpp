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
    
    std::unordered_map<std::string, sf::RectangleShape*> pixelLocations;
    sf::Vector2f cord(-100,-100);
    sf::Vector2f lastCord(-100,-100);
    sf::Vector2f diff(0,0);

    std::string cordKey = "";
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
            else if(e.type == sf::Event::MouseButtonPressed || pressed)
            {
                pressed = true;
                cord = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                std::cout << cord.x << " " << cord.y << std::endl;
                if(pan)
                    diff = cord - lastCord;
                else
                    diff = sf::Vector2f(0,0);

                cordKey = std::to_string(int(cord.x)) + "," + std::to_string(int(cord.y));
                //std::cout << cordKey << std::endl;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    pan = true;
                    //std::cout << diff.x << " " << diff.y << std::endl;
                }
                //if(!(cord.x >= 0 && cord.x < window.getSize().x && cord.y >= 0 && cord.y < window.getSize().y))
                //    pan = false;

                lastCord = cord;
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
                        std::cout << "undo squiggle!" << std::endl;
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
                    while(pixelLocations.size() > 0)
                    {
                        auto it = pixelLocations.begin();
                        delete it->second;
                        pixelLocations.erase(it->first);
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
            sf::RectangleShape* temp = new sf::RectangleShape(cord);
            temp->setSize(sf::Vector2f(penSize,penSize));
            temp->setPosition(cord);
            temp->setFillColor(penInk);
            pixelLocations.emplace(cordKey, temp);
            pixelLocations[cordKey]->setFillColor(temp->getFillColor());
            //std::cout << "drawn on screen: " << pixelLocations.size() << std::endl;
        }
        else if(pressed && eraser && !pan)
        {
            int eRange = 6;
            for(int i = -eRange; i <= eRange; i++)
            {
                sf::Vector2f tempC = cord;
                tempC.x+=i;
                for(int j = -eRange; j <= eRange; j++)
                {
                    tempC.y+=j;
                    cordKey = std::to_string(int(tempC.x)) + "," + std::to_string(int(tempC.y));
                    
                    if(pixelLocations.find(cordKey) != pixelLocations.end())
                        delete pixelLocations[cordKey];
                    pixelLocations.erase(cordKey);
                }
                
            }
        }
        else if(pan)
        {
            for(auto it = pixelLocations.begin(); it != pixelLocations.end(); it++)
                (*it->second).setPosition(it->second->getPosition()+diff);
        }

        window.clear(sf::Color::White);

        for(auto it = pixelLocations.begin(); it != pixelLocations.end(); it++)
        {
            window.draw(*(it->second));
        }
        window.display();
    }

    while(pixelLocations.size() > 0)
    {
        auto it = pixelLocations.begin();
        delete it->second;
        pixelLocations.erase(it->first);
    }
}