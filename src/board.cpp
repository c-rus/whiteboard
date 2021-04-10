#include "board.h"

Board::Board(int w, int h)
{
    width = w;
    height = h;
}

Board::~Board()
{
    clear();
}

void Board::startSqui(sf::Vector2i& loc, int w, sf::Color i)
{
    scribs.push_back(new Squiggle(loc, w, i));
}

void Board::continueSqui(sf::Vector2i& loc, int w, sf::Color i)
{
    scribs.back()->addPoint(loc, w, i);
}

void Board::pan(sf::Vector2i& offset)
{
    for(auto it = scribs.begin(); it != scribs.end(); it++)
        (*it)->move(offset);
}

void Board::erase(sf::Vector2i& loc, int w)
{

}

void Board::resize(int w, int h)
{
    width = w;
    height = h;
    std::cout << "WIDTH: " << width << std::endl;
    std::cout << "HEIGHT: " << height << std::endl;
}

void Board::draw(sf::RenderWindow& win)
{
    for(auto it = scribs.begin(); it != scribs.end(); it++)
        (*it)->draw(win);
}

void Board::clear()
{
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
}

void Board::redo()
{
    if(rScribs.size() > 0)
    {
        scribs.push_back(rScribs.back());
        rScribs.pop_back();
    }
}

void Board::undo()
{
    if(scribs.size() > 0)
    {
        rScribs.push_back(scribs.back());
        scribs.pop_back();
    }
}