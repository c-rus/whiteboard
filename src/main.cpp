#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <string>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Whiteboard");
    std::cout << "Welcome to whiteboard!" << std::endl;
    window.setFramerateLimit(60);
    
    std::unordered_map<std::string, sf::RectangleShape*> pixelLocations;
    sf::Vector2f cord(-100,-100);
    std::string cordKey = "";
    sf::Color penInk = sf::Color::Black;
    int penSize = 6;

    bool pressed = false;
    bool eraser = false;
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
            }
            else if(e.type == sf::Event::MouseButtonPressed || pressed)
            {
                pressed = true;
                cord = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                cordKey = std::to_string(int(cord.x)) + "," + std::to_string(int(cord.y));
                std::cout << cordKey << std::endl;
            }
            else if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::G)
                {
                    penInk = sf::Color::Green;
                }
                else if(e.key.code == sf::Keyboard::B)
                {
                    penInk = sf::Color::Black;
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
            }
        }


        if(pressed && !eraser)
        {
            sf::RectangleShape* temp = new sf::RectangleShape(cord);
            temp->setSize(sf::Vector2f(penSize,penSize));
            temp->setPosition(cord);
            temp->setFillColor(penInk);
            pixelLocations.emplace(cordKey, temp);
        }
        else if(pressed && eraser)
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

        window.clear(sf::Color::White);

        for(auto it = pixelLocations.begin(); it != pixelLocations.end(); it++)
        {
            window.draw(*(it->second));
        }
        window.display();
    }
}