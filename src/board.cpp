#include "board.h"

Board::Board(int w, int h)
{
    width = w;
    height = h;
    frameBuffer = 0;
    refresh = false;
    frame = Box(0, 0, width, height);
}

Board::~Board()
{
    clear();
}

void Board::startSqui(sf::Vector2i& loc, int w, sf::Color i)
{
    scribs.push_back(new Squiggle(loc, w, i));
    refresh = true;
    visibleScribs.push_back(scribs.back());
}

void Board::continueSqui(sf::Vector2i& loc, int w, sf::Color i)
{
    refresh = refresh || scribs.back()->addPoint(loc, w, i);
}

void Board::pan(sf::Vector2i& offset)
{
    refresh = true;
    visibleScribs.clear();
    for(auto it = scribs.begin(); it != scribs.end(); it++)
    {
        (*it)->move(offset);
        if(frame.contains((*it)->getBounds()))
            visibleScribs.push_back(*it);
    }
    
    for(auto it = rScribs.begin(); it != rScribs.end(); it++)
    {
        (*it)->move(offset);
    }
}

void Board::erase(sf::Vector2i& loc, int w)
{
    //testing bounds checking
    if(!scribs.empty())
    {
        scribs.front()->getBounds().print();
        frame.print();
        if(frame.contains(scribs.front()->getBounds()))
            std::cout << "scribble is inside the window" << std::endl;
        else
            std::cout << "scribble out of bounds" << std::endl;
    }
}

void Board::resize(int w, int h)
{
    refresh = true;
    width = w;
    height = h;
    frame = Box(0, 0, width, height);
    std::cout << "WIDTH: " << width << std::endl;
    std::cout << "HEIGHT: " << height << std::endl;
}

void Board::draw(sf::RenderWindow& win)
{
    if(refresh || frameBuffer < 2)
    {
        if(refresh) 
        {
            frameBuffer = 0;
        }
        std::cout << "refreshing" << frameBuffer << std::endl;

        win.clear(sf::Color::White);
        for(auto it = visibleScribs.begin(); it != visibleScribs.end(); it++)
            (*it)->draw(win); 

        frameBuffer++;
    }

}

void Board::clear()
{
    refresh = true;
    while(!scribs.empty())
    {
        delete scribs.back();
        scribs.pop_back();
    }
    while(!rScribs.empty())
    {
        delete rScribs.back();
        rScribs.pop_back();
    }
    visibleScribs.clear();
}

void Board::redo()
{
    refresh = true;
    if(rScribs.size() > 0)
    {
        scribs.push_back(rScribs.back());
        if(frame.contains(rScribs.back()->getBounds()))
            visibleScribs.push_back(rScribs.back());
        rScribs.pop_back();
    }
}

void Board::undo()
{
    refresh = true;
    if(scribs.size() > 0)
    {
        rScribs.push_back(scribs.back());
        if(scribs.back() == visibleScribs.back())
            visibleScribs.pop_back();
        scribs.pop_back();
    }
}

void Board::deassertRefresh()
{
    refresh = false;
}