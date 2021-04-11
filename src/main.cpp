#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "squiggle.h"
#include "stylus.h"
#include "board.h"
#include "clickable.h"
#include "fileManager.h"
#include "hud.h"

/*
TODO: key outlooks
    - draw only top-most pixel (use unordered_map or array)
    - draw only visible pixels (not offscreen pixels)
    -everytime things are moved, try to fit into board's array (start with top squiggles)
*/
int main(int argc, char ** argv)
{
    std::string path = "./";
    FileManager fm(path);

    std::string fileName = "";
    for(int i = 0; i < argc; i++)
    {
        if(i == 1) //try to load this file path
            fileName = std::string(argv[i]);
    }

    std::string title = "Whiteboard";
    int screenWidth = 1200;
    int screenHeight = 900;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), title);
    sf::RenderTexture surf;
    surf.create(screenWidth, screenHeight);

    std::cout << "Welcome to whiteboard!" << std::endl;
    window.setFramerateLimit(60);
    sf::Vector2i loc, prevLoc, diff;

    Stylus pen(5, sf::Color(0,0,0));

    Board* canvas = fm.load(fileName, screenWidth, screenHeight);
    HUD hud(screenWidth, screenHeight);

    bool pressed, pan, cmd;
    bool upToDate = true;
    pressed = pan = cmd = false;
    while(window.isOpen())
    {
        if(canvas->isRefreshing())
            upToDate = false;
            
        canvas->deassertRefresh();

        if(!upToDate)
            window.setTitle(canvas->getName() + "* - " + title);
        else
            window.setTitle(canvas->getName() + " - " + title);

        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
            else if(e.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, e.size.width, e.size.height);
                canvas->resize(visibleArea.width, visibleArea.height);    
                surf.create(visibleArea.width, visibleArea.height);
                window.setView(sf::View(visibleArea));
            }
            else if(e.type == sf::Event::MouseButtonReleased)
            {
                pressed = pan = false;
            }
            else if(!pressed && !pan && e.type == sf::Event::MouseMoved)
            {
                loc = sf::Mouse::getPosition(window);
                if(hud.inspect(loc))
                    canvas->assertRefresh();
            }
            else if(e.type == sf::Event::MouseButtonPressed)
            {   
                
                loc = sf::Mouse::getPosition(window);
                if(hud.interact(loc, *canvas))
                    continue;
                
                pressed = true;
                prevLoc = loc;
                diff = sf::Vector2i(0,0);

                if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    pan = true;
                else if(pen.getMode() == Stylus::DRAW)
                    canvas->startSqui(loc, pen.getWidth(), pen.getInk());
            }
            else if(e.type == sf::Event::MouseWheelMoved)
            {
                //std::cout << e.mouseWheel.delta << std::endl;
                loc = sf::Mouse::getPosition(window);
                sf::Vector2i scrollPace(0, e.mouseWheel.delta*4);
                canvas->pan(scrollPace);
            }
            else if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::LSystem) cmd = true;
                else if(e.key.code == sf::Keyboard::Z && cmd) canvas->undo();
                else if(e.key.code == sf::Keyboard::X && cmd) canvas->redo();
                else if(e.key.code == sf::Keyboard::S && cmd) 
                {
                    if(fm.save(*canvas))
                    {
                        std::cout << "Save successful!" << std::endl;
                        upToDate = true;
                    }
                    else
                        std::cout << "Save failed!" << std::endl;
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
                else if(e.key.code == sf::Keyboard::A)
                {
                    //tests resizing using hot-keys
                    int newW = sf::VideoMode::getDesktopMode().width;
                    int newH = sf::VideoMode::getDesktopMode().height;
                    sf::FloatRect visibleArea(0, 0, newW, newH);
                    auto pos = window.getPosition();
                    canvas->resize(visibleArea.width, visibleArea.height);
                    window.setSize(sf::Vector2u(newW, newH));
                    window.setView(sf::View(visibleArea));
                    window.setPosition(pos);
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
                else if(e.key.code == sf::Keyboard::C) canvas->clear();
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
            canvas->pan(diff);
            prevLoc = loc;
        }
        else if(pressed)
        {
            switch(pen.getMode())
            {
                case Stylus::DRAW:
                    canvas->continueSqui(loc, pen.getWidth(), pen.getInk());
                    break;
                case Stylus::ERASE:
                    canvas->erase(loc, pen.getWidth());
                    break;
                default:
                    std::cout << "ERROR: Unknown mode." << std::endl;
            }
        }

        hud.draw(surf);
        canvas->draw(window);
        
        window.draw(hud.getLayer(surf));
        window.display();
    }

    delete canvas;
}