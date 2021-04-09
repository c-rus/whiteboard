#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Whiteboard");
    std::cout << "Welcome to whiteboard!" << std::endl;
    std::list<sf::RectangleShape> pixels;
    bool pressed = false;
    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
            else if(e.type == sf::Event::MouseButtonPressed)
            {
                pressed = true;
            }
            else if(e.type == sf::Event::MouseButtonReleased)
            {
                pressed = false;
            }
        }


        if(pressed)
        {
            sf::Vector2f cord = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            sf::RectangleShape temp(cord);
            temp.setSize(sf::Vector2f(10,10));
            temp.setPosition(cord);
            temp.setFillColor(sf::Color::Black);
            pixels.push_back(temp);
        }

        window.clear(sf::Color::White);

        for(auto it = pixels.begin(); it != pixels.end(); it++)
        {
            window.draw(*it);
        }
        window.display();
    }
}