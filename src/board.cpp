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
    extraIsoDistance = (width+(2*gridWidth))/(0.5);
    gridVisible = false;
    linesVisible = false;
    isometricVisible = false;
    selecting = false;
    isStraight = false;
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
    isometricVisible  = false;
    gridVisible = !gridVisible;
}

void Board::switchIsometric()
{
    linesVisible = false;
    gridVisible = false;
    isometricVisible = !isometricVisible;
}

void Board::switchLines()
{
    gridVisible = false;
    isometricVisible = false;
    linesVisible = !linesVisible;
}

void Board::startSelection(sf::Vector2i& loc)
{
    selecting = true;
    selector.shift(loc.x-selector.getX(), loc.y-selector.getY());
    selector.setVisibility(true);
}

void Board::clearSelection()
{
    for(auto it = visibleScribs.begin(); it != visibleScribs.end(); it++)
    {
        if((*it)->getBounds().isVisible())
            selecting = false;
        (*it)->getBounds().setVisibility(false);
    }
}

bool Board::isSelected()
{
    return selecting;
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
    selecting = false;
    for(auto it = visibleScribs.begin(); it != visibleScribs.end(); it++)
    {
        if(selector.intersect((*it)->getBounds()))
        {
            (*it)->getBounds().setVisibility(true);
            selecting = true;
        }
    }
}

void Board::startSqui(sf::Vector2i& loc, int w, Color c)
{
    if(c == getBackdrop())
        std::cout << "erasing!" << std::endl;
        
    scribs.push_back(new Squiggle(loc, w, c));
    visibleScribs.push_back(scribs.back());
    refresh = true;
    scribbling = true;
}

void Board::continueSqui(sf::Vector2i& loc, int w, Color c)
{
    refresh = scribs.back()->addPoint(loc, w, c);
}

void Board::toggleStraightEdge(bool b)
{
    isStraight = b;
}

void Board::compressSqui()
{
    scribs.back()->compress(isStraight);
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

Color& Board::getBackdrop()
{
    return backdrop;
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
    
    //update visibility area
    visibleScribs.clear();
    for(auto it = scribs.begin(); it != scribs.end(); it++)
    {
        if(frame.intersect((*it)->getBounds()))
            visibleScribs.push_back(*it);
    }
    extraIsoDistance = (width+(2*gridWidth))/(0.5);
}

void Board::draw(sf::RenderWindow& win)
{
    win.clear(backdrop.getSFColor());
    
    if(gridVisible || isometricVisible)
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
    if(isometricVisible)
    {
        for(int j = -int(extraIsoDistance); j <= gridWidth+width; j+=gridWidth)
        {
            //draw downward slopes
            sf::RectangleShape isoline(sf::Vector2f(1, extraIsoDistance));
            isoline.setPosition(j+backOffset.x, -gridWidth+backOffset.y);
            isoline.setFillColor(Color(Color::Gray).getSFColor());
            isoline.setRotation(-60);
            win.draw(isoline);
        }
        for(int j = 0; j <= int(extraIsoDistance)+width; j+=gridWidth)
        {
            //draw upward slopes
            sf::RectangleShape isoline(sf::Vector2f(1, extraIsoDistance));
            isoline.setPosition(j+backOffset.x, -gridWidth+backOffset.y);
            isoline.setFillColor(Color(Color::Gray).getSFColor());
            isoline.setRotation(60);
            win.draw(isoline);
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
    //read in grid coordinates
    char offset = 0;
    file.read((char*)&offset, sizeof(offset));
    backOffset.x = offset;
    file.read((char*)&offset, sizeof(offset));
    backOffset.y = offset;
    
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
    //save backoffset coordinates
    char gridPos = backOffset.x;
    file.write((char*)&gridPos, sizeof(gridPos));
    gridPos = backOffset.y;
    file.write((char*)&gridPos, sizeof(gridPos));

    //how many squiggles?
    int sCount = scribs.size();
    file.write((char*)&sCount, sizeof(sCount));

    //write all squiggles
    for(auto it = scribs.begin(); it != scribs.end(); it++)
    {
        (*it)->save(file);
    }
}