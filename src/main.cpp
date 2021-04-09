#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "Whiteboard");
    std::cout << "Welcome to whiteboard!" << std::endl;
    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
        }
    }
}