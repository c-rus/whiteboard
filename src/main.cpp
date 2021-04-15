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
    - selection tool, draw a box and see if it contains a squiggle's bounds
    - background canvas
    - open recent option (save settings to a file thats read on startup)
    - eraser feature (eraser is just drawing with the background color)
    - huffman encoding for pixel colors
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

    std::cout << "Welcome to whiteboard!" << std::endl;
    window.setFramerateLimit(60);
    sf::Vector2i loc, prevLoc, diff;
    int scrollPace = 0;

    std::string name = "";
    std::string alert = "";
    bool capsLock = false;

    Stylus pen(2, Color::Black);
    window.setMouseCursor(pen.swapMode(pen.getMode())); //set initial cursor

    Board* canvas = fm.load(fileName, screenWidth, screenHeight);
    HUD hud(screenWidth, screenHeight);

    bool pressed, pan, cmd, scrolling;
    bool askforTitle = false;
    bool upToDate = true;
    pressed = pan = cmd = scrolling = false;
    while(window.isOpen())
    {
        if(canvas->isRefreshing())
            upToDate = false;
            
        canvas->deassertRefresh();
        scrolling = false;

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
                    std::cout << "Save successful!" << std::endl;
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
                hud.inspect(loc);
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
                {
                    //center the brush on the cursor
                    loc.x = loc.x - pen.getRadius();
                    loc.y = loc.y - pen.getRadius();
                    canvas->startSqui(loc, pen.getRadius(), pen.getInk());
                }
                else if(pen.getMode() == Stylus::ERASE)
                {
                    //center the brush on the cursor
                    loc.x = loc.x - pen.getRadius();
                    loc.y = loc.y - pen.getRadius();
                    canvas->startSqui(loc, pen.getRadius(), Color::White);
                }
                    
            }
            else if(e.type == sf::Event::MouseWheelMoved)
            {
                int maxPace = 50;
                int direction = (e.mouseWheel.delta < 0) ? -1 : 1;
                if(scrollPace != 0 && scrollPace/abs(scrollPace) != direction)
                    direction = -direction;
                scrollPace = 2*direction*e.mouseWheel.delta*e.mouseWheel.delta;
                
                if(abs(scrollPace) > maxPace)
                    scrollPace = maxPace*direction;

                scrolling = true;
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
                else if(e.key.code == sf::Keyboard::W)
                {
                    //calculate origin
                    sf::Vector2f origin(float(window.getSize().x/2), float(window.getSize().y/2));
                    //canvas->zoom(1, origin);
                }
                else if(e.key.code == sf::Keyboard::Y)
                {
                    sf::Vector2f origin(float(window.getSize().x/2), float(window.getSize().y/2));
                    //canvas->zoom(-1, origin);
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
                    pen.setRadius(pen.getRadius()+1);
                }
                else if(e.key.code == sf::Keyboard::Down)
                {
                    pen.setRadius(pen.getRadius()-1);
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
        if(pan || scrolling)
        {
            if(scrolling)
            {
                sf::Vector2i paceVec(0, scrollPace);
                canvas->pan(paceVec);  
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
            loc.x = loc.x - pen.getRadius();
            loc.y = loc.y - pen.getRadius();
            switch(pen.getMode())
            { 
                case Stylus::DRAW:
                    canvas->continueSqui(loc, pen.getRadius(), pen.getInk());
                    break;
                case Stylus::ERASE:
                    canvas->continueSqui(loc, pen.getRadius(), Color::White);
                    break;
                default:
                    std::cout << "ERROR: Unknown mode." << std::endl;
            }
        }

        
        canvas->draw(window);
        hud.draw(window);
        window.display();
    }

    delete canvas;
}