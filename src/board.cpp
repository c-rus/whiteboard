#include "board.h"

Board::Board(int w, int h, std::string title)
{
    width = w;
    height = h;
    name = title;
    refresh = false;
    frame = Box(0, 0, width, height);
    container = Box(0,0);
    backdrop = Color::White;
    gridWidth = 40;
    gridVisible = false;
    linesVisible = false;
}

Board::~Board()
{
    clear();
}

void Board::update()
{
    
}

void Board::switchGrid()
{
    linesVisible = false;
    gridVisible = !gridVisible;
}

void Board::switchLines()
{
    gridVisible = false;
    linesVisible = !linesVisible;
}

void Board::startSelection(sf::Vector2i& loc)
{
    selector.shift(loc.x-selector.getX(), loc.y-selector.getY());
    selector.setVisibility(true);
}

void Board::continueSelection(sf::Vector2i& loc)
{
    auto& b = selector;
    b.adjust(loc.x-b.getOutline().getPosition().x, loc.y-b.getOutline().getPosition().y);
}

void Board::grabSelection()
{
    selector.setVisibility(false);
    selector.print();
    for(auto it = visibleScribs.begin(); it != visibleScribs.end(); it++)
    {
        if(selector.intersect((*it)->getBounds()))
            (*it)->getBounds().setVisibility(true);
    }
}

void Board::startSqui(sf::Vector2i& loc, int w, Color c)
{
    scribs.push_back(new Squiggle(loc, w, c));
    visibleScribs.push_back(scribs.back());
    refresh = true;
    scribbling = true;
}

void Board::continueSqui(sf::Vector2i& loc, int w, Color c)
{
    refresh = scribs.back()->addPoint(loc, w, c);
}

void Board::compressSqui()
{
    scribs.back()->compress();
    //update container
    Box& b = scribs.back()->getBounds();
    if(container.getWidth() == -1 && container.getHeight() == -1)
        container = b;
    else
    {
        container.stretch(b.getX(), b.getY());
        container.stretch(b.getX()+b.getWidth(), b.getY()+b.getHeight());
    }
    refresh = true;
}

void Board::zoom(int scale, sf::Vector2f& origin)
{
    for(auto it = scribs.begin(); it != scribs.end(); it++)
    {
        //calculate distance from center of screen
        (*it)->zoom(scale, origin);
    }
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
        (*it)->move(offset);

    container.shift(offset.x, offset.y);
    backOffset.x = (backOffset.x + offset.x) % gridWidth;
    backOffset.y = (backOffset.y + offset.y) % gridWidth;
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
    win.clear(backdrop.getSFColor());
    
    if(gridVisible)
    {
        for(int i = -gridWidth; i <= width; i+=gridWidth)
        {
            sf::RectangleShape gridline(sf::Vector2f(1, height+(2*gridWidth)));
            gridline.setPosition(i+backOffset.x, -gridWidth+backOffset.y);
            gridline.setFillColor(Color(Color::Gray).getSFColor());
            win.draw(gridline);
        }
    }
    if(linesVisible || gridVisible)
    {
        for(int j = -gridWidth; j <= height; j+=gridWidth)
        {
            sf::RectangleShape gridline(sf::Vector2f(width+(2*gridWidth), 1));
            gridline.setPosition(-gridWidth+backOffset.x, j+backOffset.y);
            gridline.setFillColor(Color(Color::Gray).getSFColor());
            win.draw(gridline);
        }
    }

    for(auto it = visibleScribs.begin(); it != visibleScribs.end(); it++)
        (*it)->draw(win); 
    selector.draw(win);
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
        //int i = scribs.back()->count();
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
        compressSqui();
    }
    deassertRefresh();
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