#include "board.h"

Board::Board(int w, int h, std::string title)
{
    width = w;
    height = h;
    name = title;
    refresh = false;
    frame = Box(0, 0, width, height);
}

Board::~Board()
{
    clear();
}

void Board::startSqui(sf::Vector2i& loc, int w, Color c)
{
    scribs.push_back(new Squiggle(loc, w, c));
    refresh = true;
    visibleScribs.push_back(scribs.back());
}

void Board::continueSqui(sf::Vector2i& loc, int w, Color c)
{
    refresh = refresh || scribs.back()->addPoint(loc, w, c);
}

void Board::compressSqui()
{
    scribs.back()->compress();
    refresh = true;
}

void Board::pan(sf::Vector2i& offset)
{
    refresh = true;
    visibleScribs.clear();
    for(auto it = scribs.begin(); it != scribs.end(); it++)
    {
        (*it)->move(offset);
        if(frame.intersect((*it)->getBounds()))
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
        if(frame.intersect(scribs.front()->getBounds()))
            std::cout << "scribble is visible" << std::endl;
        else
            std::cout << "scribble is out of bounds" << std::endl;
    }
}

void Board::resize(int w, int h)
{
    if(w > MAX_W)
        width = MAX_W;
    else
        width = w;

    if(h > MAX_H)
        height = MAX_H;
    else
        height = h;

    frame = Box(0, 0, width, height);
    std::cout << "WIDTH: " << width << std::endl;
    std::cout << "HEIGHT: " << height << std::endl;
}

void Board::draw(sf::RenderWindow& win)
{
    win.clear(sf::Color::White);
    for(auto it = visibleScribs.begin(); it != visibleScribs.end(); it++)
        (*it)->draw(win); 
}

void Board::setName(std::string& n)
{
    name = n;
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

void Board::assertRefresh()
{
    refresh = true;
}

std::string& Board::getName()
{
    return name;
}

bool Board::isRefreshing()
{
    return refresh;
}

Board::Board(std::fstream& file, int w, int h, std::string title) : Board(w, h, title)
{
    int sCount = 0;
    file.read((char*)&sCount, sizeof(sCount));
    for(int i = 0; i < sCount; i++)
    {
        scribs.push_back(new Squiggle(file));
        if(frame.contains(scribs.back()->getBounds()))
            visibleScribs.push_back(scribs.back());
    }
}

void Board::save(std::fstream& file)
{
    //how many squiggles?
    int sCount = scribs.size();
    file.write((char*)&sCount, sizeof(sCount));
    //write all squiggles
    for(auto it = scribs.begin(); it != scribs.end(); it++)
    {
        (*it)->save(file);
    }
}