#pragma once
#include <list>
#include "squiggle.h"
#include <iostream>

class Board
{
private:
    std::list<Squiggle*> scribs;
    std::list<Squiggle*> rScribs;
    std::list<Squiggle*> visibleScribs;

    Color backdrop;
    sf::Vector2i backOffset;
    int gridWidth;
    bool gridVisible;
    bool linesVisible;

    std::string name;
    bool refresh;
    Box frame; //the visible window
    Box container; //the boundaries of the entire drawn art
    Box selector; //selection box
    int width;
    int height;
    bool scribbling;
    bool selecting;

    const int MAX_W = sf::VideoMode::getDesktopMode().width;
    const int MAX_H = sf::VideoMode::getDesktopMode().height;

public:
    Board(int w, int h, std::string title="");
    Board(std::fstream& file, int w, int h, std::string title); //loading a board
    ~Board();

    void update(); //TODO implement
    
    void startSqui(sf::Vector2i& loc, int w, Color c);
    void continueSqui(sf::Vector2i& loc, int w, Color c);
    void compressSqui();

    void draw(sf::RenderWindow& win);
    void zoom(int scale, sf::Vector2f& origin);
    void pan(sf::Vector2i& offset);

    //light erase- only erase top-level squiggle components
    //deep erase- erase all-level squiggle components
    //squi erase- erase the whole squiggle thats in contact with eraser
    
    //tip: if the squiggle boundary contains that eraser squiggle->
    //draw the eraser squiggle inside the squiggle boundaries
    void erase(sf::Vector2i& loc, int w);

    void resize(int w, int h);
    void redo();
    void undo();
    void clear();

    void startSelection(sf::Vector2i& loc);
    void continueSelection(sf::Vector2i& loc);
    void grabSelection();
    bool isSelected();
    void clearSelection();

    void deassertRefresh();
    void assertRefresh();
    bool isRefreshing();

    void switchGrid();
    void switchLines();

    std::string& getName();
    void setName(std::string& n);

    void save(std::fstream& file);
};