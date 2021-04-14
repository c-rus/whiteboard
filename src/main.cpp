#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include "squiggle.h"
#include "stylus.h"
#include "board.h"
#include "color.h"
#include "clickable.h"
#include "fileManager.h"
#include "hud.h"

/*
TODO: key outlooks
    - draw only top-most pixel (use unordered_map or array)
    - open recent option (save settings to a file thats read on startup)
    - eraser feature (eraser is just drawing with the background color)
    -everytime things are moved, try to fit into board's array (start with top squiggles)
*/
int main(int argc, char ** argv)
{
    std::string path = "./build/";
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
    sf::Image icn;
    icn.loadFromFile("./assets/icon.png");
    window.setIcon(32, 32, icn.getPixelsPtr());
    sf::RenderTexture surf;
    surf.create(screenWidth, screenHeight);

    std::cout << "Welcome to whiteboard!" << std::endl;
    window.setFramerateLimit(60);
    sf::Vector2i loc, prevLoc, diff;
    std::queue<int> scrollPaces;
    int lastSpeed = 0;

    std::string name = "";
    std::string alert = "";
    bool capsLock = false;

    Stylus pen(4, Color(0,0,0));
    window.setMouseCursor(pen.swapMode(pen.getMode())); //set initial cursor

    Board* canvas = fm.load(fileName, screenWidth, screenHeight);
    HUD hud(screenWidth, screenHeight);

    bool pressed, pan, cmd;
    bool askforTitle = false;
    bool upToDate = true;
    pressed = pan = cmd = false;
    while(window.isOpen())
    {
        if(canvas->isRefreshing())
            upToDate = false;
            
        canvas->deassertRefresh();

        if(!upToDate)
            window.setTitle(canvas->getName() + "* - " + title + alert);
        else
            window.setTitle(canvas->getName() + " - " + title);

        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
            else if(askforTitle && e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::LShift || e.key.code == sf::Keyboard::RShift)
                    capsLock = true;
                else if(e.key.code == sf::Keyboard::BackSpace && !name.empty())
                    name.pop_back();
                else if(e.key.code == sf::Keyboard::Enter && !name.empty())
                {
                    askforTitle = false;
                    alert = "";
                    upToDate = fm.save(*canvas, true);
                }
                else if(e.key.code >= sf::Keyboard::A && e.key.code <= sf::Keyboard::Z)
                {
                    int offset = (capsLock) ? 65 : 97; 
                    name.push_back(char((e.key.code)+offset));
                }
                else if(e.key.code >= sf::Keyboard::Num0 && e.key.code <= sf::Keyboard::Num9)
                {
                    int offset = 22;
                    name.push_back(char((e.key.code)+offset));
                }
                
                canvas->setName(name);
            }
            else if(e.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, e.size.width, e.size.height);
                canvas->resize(visibleArea.width, visibleArea.height);    
                surf.create(visibleArea.width, visibleArea.height);
                window.setView(sf::View(visibleArea));
            }
            else if(e.type == sf::Event::MouseButtonReleased)
            {
                if(pressed && !pan) //finished a squiggle
                    canvas->compressSqui();
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
                else if(pen.getMode() == Stylus::ERASE)
                    canvas->startSqui(loc, pen.getWidth(), Color::White);
            }
            else if(e.type == sf::Event::MouseWheelMoved)
            {
                //TODO: Smoother scrolling
                int direction = (e.mouseWheel.delta < 0) ? -1 : 1;
                if(lastSpeed != 0 && lastSpeed/abs(lastSpeed) != direction)
                    direction = -direction;
                //int pace = 2*direction*e.mouseWheel.delta*e.mouseWheel.delta + 4*e.mouseWheel.delta + 1*direction;
                int pace = 2*direction*e.mouseWheel.delta*e.mouseWheel.delta;
                //std::cout << pace << std::endl;
                scrollPaces.push(pace);
            }
            else if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::LSystem) cmd = true;
                else if(e.key.code == sf::Keyboard::Z && cmd) canvas->undo();
                else if(e.key.code == sf::Keyboard::X && cmd) canvas->redo();
                else if(e.key.code == sf::Keyboard::S && cmd) 
                {
                    askforTitle = !fm.save(*canvas);
                    if(!askforTitle)
                    {
                        std::cout << "Save successful!" << std::endl;
                        upToDate = true;
                    }
                    else
                    {
                        alert = " (Type a name to save...)";
                        std::cout << "Save failed!" << std::endl;
                    }
                }
                else if(e.key.code == sf::Keyboard::G)
                {
                    pen.setInk(Color::GreenAlt);
                }
                else if(e.key.code == sf::Keyboard::B)
                {
                    pen.setInk(Color::Black);
                }
                else if(e.key.code == sf::Keyboard::O)
                {
                    pen.setInk(Color::Orange);
                }
                else if(e.key.code == sf::Keyboard::R)
                {
                    pen.setInk(Color::Red);
                }
                else if(e.key.code == sf::Keyboard::L)
                {
                    pen.setInk(Color::Blue);
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
                    window.setMouseCursor(pen.swapMode(Stylus::ERASE));
                }
                else if(e.key.code == sf::Keyboard::D)
                {
                    window.setMouseCursor(pen.swapMode(Stylus::DRAW));
                }
                else if(e.key.code == sf::Keyboard::C) canvas->clear();
            }
            else if(e.type == sf::Event::KeyReleased)
            {
                if(e.key.code == sf::Keyboard::LSystem) cmd = false;

                if(e.key.code == sf::Keyboard::LShift || 
                   e.key.code == sf::Keyboard::RShift) capsLock = false;
            }
        }

        hud.update();

        loc = sf::Mouse::getPosition(window);
        if(pan || !scrollPaces.empty())
        {
            if(!scrollPaces.empty())
            {
                lastSpeed = scrollPaces.front();
                scrollPaces.pop(); 
                sf::Vector2i scrollPace(0, lastSpeed);
                canvas->pan(scrollPace);  
            }
            else
            {
                diff = loc - prevLoc;
                canvas->pan(diff);
                prevLoc = loc;
            }

        }
        else if(pressed)
        {
            switch(pen.getMode())
            {
                case Stylus::DRAW:
                    canvas->continueSqui(loc, pen.getWidth(), pen.getInk());
                    break;
                case Stylus::ERASE:
                    canvas->continueSqui(loc, pen.getWidth(), Color::White);
                    //canvas->erase(loc, pen.getWidth());
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