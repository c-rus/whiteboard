#pragma once
#include <list>
#include "squiggle.h"
#include <iostream>
#include <unordered_map>

class Board
{
private:
    //std::list<Squiggle*>::iterator furthestMark; //currently not in-use
    std::list<Squiggle*> scribs;
    std::list<Squiggle*> rScribs;
    std::string name;
    
    std::list<Squiggle*> visibleScribs;
    bool refresh;
    int frameBuffer;

    Box frame;

    int width;
    int height;

    const int MAX_W = sf::VideoMode::getDesktopMode().width;
    const int MAX_H = sf::VideoMode::getDesktopMode().height;

public:
    Board(int w, int h, std::string title="");
    Board(std::fstream& file, int w, int h, std::string title); //loading a board
    ~Board();

    void startSqui(sf::Vector2i& loc, int w, Color c);
    void continueSqui(sf::Vector2i& loc, int w, Color c);

    void draw(sf::RenderWindow& win);

    void pan(sf::Vector2i& offset);

    //light erase- only erase top-level squiggle components
    //deep erase- erase all-level squiggle components
    //squi erase- erase the whole squiggle thats in contact with eraser
    void erase(sf::Vector2i& loc, int w);

    void resize(int w, int h);
    void redo();
    void undo();
    void clear();

    void deassertRefresh();
    void assertRefresh();
    bool isRefreshing();

    std::string& getName();
    void setName(std::string& n);

    void save(std::fstream& file);
};