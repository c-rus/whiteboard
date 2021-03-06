#include "squiggle.h"

Squiggle::Squiggle(sf::Vector2i& start, int radius, Color c)
{
    this->prev = sf::Vector2f(start);
    lines.push_back(new Pixel(prev, radius, c));
    bounds = Box(start.x, start.y, radius*2, radius*2);
    optimized = false;
    scalar = 1;
    points.push_back(std::make_pair(start, radius));
}

Squiggle::~Squiggle()
{
    while(!lines.empty())
    {
        delete lines.back();
        lines.pop_back();
    }
    delete rt;
    delete sp;
}

void Squiggle::draw(sf::RenderWindow& win)
{
    bounds.draw(win);
    if(optimized)
        win.draw(*sp);
    else
    {
        for(auto it = lines.begin(); it != lines.end(); it++)
            win.draw((*it)->getDot());
    }
}

//TODO: Work on fixing zoom 
void Squiggle::zoom(int scale, sf::Vector2f& origin)
{
    int quad = 0;
    int offX = 0;
    int offY = 0;
    if(sp->getPosition().x >= origin.x)
    {
        if(sp->getPosition().y <= origin.y)
        {
            quad = 1;
            offY = bounds.getHeight();
        }
        else
        {
            quad = 4;
        }
    }
    else
    {
        if(sp->getPosition().y <= origin.y)
        {
            quad = 2;
            offX = bounds.getWidth();
            offY = bounds.getHeight();
        }
        else
        {
            offX = bounds.getWidth();
            quad = 3;
        }
    }
    sf::Vector2f prevSize = sf::Vector2f(sp->getGlobalBounds().width, sp->getGlobalBounds().height);
    //calculate difference between endpoints
    sf::Vector2f dist = sf::Vector2f(sp->getPosition().x+offX, sp->getPosition().y+offY)-origin;
    scalar -= scale;
    float f = 1;
    if(scale == 1)
        f = 2*(sp->getScale().x+0.5);
    else
        f = 0.5*(sp->getScale().x);
    sp->setScale(sf::Vector2f(f, f));
    sf::Vector2f size = sf::Vector2f(sp->getGlobalBounds().width, sp->getGlobalBounds().height);
    std::cout << "difference in width: " << size.x-prevSize.x << std::endl;
    sf::Vector2f diff = size - prevSize;
    //wrong implementation-
    if(scale == 1)
        sp->setPosition(sp->getPosition()+sf::Vector2f(2*dist.x, 2*dist.y));
    else
        sp->setPosition(sp->getPosition()-sf::Vector2f(0.5*dist.x, 0.5*dist.y));
    /*if(scale == -1)
        sp->setPosition((sp->getPosition().x+origin.x)/2, (sp->getPosition().y+origin.y)/2);
    else
        sp->setPosition(sp->getPosition().x*2, sp->getPosition().y*2);
    */
    //square both values
    dist.x = dist.x*dist.x;
    dist.y = dist.y*dist.y;
    //take the square root of the sum of x and y
    float mag = sqrt(dist.x+dist.y);
    std::cout << "distance from the center of the page: " << mag << std::endl;
    //everything must be zooming out from the origin (centerpoint)
}

void Squiggle::move(sf::Vector2i& offset)
{
    if(optimized) 
        sp->setPosition(sp->getPosition()+sf::Vector2f(offset));
    else
    {
        for(auto it = lines.begin(); it != lines.end(); it++)
        {
            auto& l = (*it)->getDot();
            l.setPosition(l.getPosition()+sf::Vector2f(offset));
        }
    }

    //update points locations
    for(auto it = points.begin(); it != points.end(); it++)
    {
        (*it).first.x += offset.x;
        (*it).first.y += offset.y;
    }

    bounds.shift(offset.x, offset.y);
}

void Squiggle::compress(bool isStraight)
{
    //create render texture
    rt = new sf::RenderTexture();

    //only connect the beginning and end points
    if(isStraight)
    {
        //capture initial point
        Pixel* initial = new Pixel(lines.front()->getLocation(), lines.front()->getRadius(), lines.front()->getColor());
        //capture endpoint
        Pixel* final = new Pixel(lines.back()->getLocation(), lines.back()->getRadius(), lines.back()->getColor());

        //reset lines list
        while(!lines.empty())
        {
            delete lines.back();
            lines.pop_back();
        }
        //reset bounds
        bounds = Box(initial->getLocation().x, initial->getLocation().y, initial->getRadius()*2, initial->getRadius()*2);
        //add the initial point
        lines.push_back(initial);
        points.push_back(std::make_pair(sf::Vector2i(initial->getLocation().x, initial->getLocation().y), initial->getRadius()));
        this->prev = initial->getLocation();
        //add intermediate points between beginning and final points
        sf::Vector2i pos = sf::Vector2i(int(final->getLocation().x), int(final->getLocation().y));
        this->addPoint(pos, final->getRadius(), final->getColor());
    }

    rt->create(bounds.getWidth(), bounds.getHeight());
    rt->clear(Color(0,0,0,0).getSFColor());
    
    //traverse the list of pixels into a single texture
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        Pixel& p = (**it);
        p.setLocation(p.getLocation()-sf::Vector2f(bounds.getX(), bounds.getY()));
        rt->draw(p.getDot());
        //delete the dynamically allocated sfml shape (no longer needed)
        (*it)->drop(); 
    }

    //set the texture to a sprite to handle for future drawing to screen
    rt->display();
    sp = new sf::Sprite();
    sp->setTexture(rt->getTexture());
    sp->setPosition(bounds.getX(), bounds.getY());
    
    optimized = true;
}

bool Squiggle::addPoint(sf::Vector2i& p, int r, Color c)
{
    if(prev == sf::Vector2f(p))
        return false;
    //calculate slopes
    float slopeYoverX = 0;
    float slopeXoverY = 0;
    if(prev.x > p.x)
        slopeYoverX = float(float(p.y-prev.y)/(float(prev.x-p.x)));
    else if(prev.x < p.x)
        slopeYoverX = float(float(p.y-prev.y)/(float(p.x-prev.x)));

    if(prev.y > p.y)
        slopeXoverY = float(float(p.x-prev.x)/(float(prev.y-p.y)));
    else if(prev.y < p.y)
        slopeXoverY = float(float(p.x-prev.x)/(float(p.y-prev.y)));

    //determine the easier slope to work with
    bool xAxis = !(abs(int(slopeYoverX)) > abs(int(slopeXoverY)));

    //choose x-axis as independent
    if(prev.x != p.x && (xAxis || prev.y == p.y))
    {
        while(prev.x != p.x)
        {
            bool headedRight = (prev.x < p.x);
            bool headedDown =  (prev.y < p.y);

            prev.x = (prev.x < p.x) ? prev.x+1 : prev.x-1;
            prev.y += slopeYoverX;

            lines.push_back(new Pixel(prev, r, c));
            int borderX = (headedRight) ? prev.x+(2*r) : prev.x;
            int borderY = (headedDown) ? prev.y+(2*r) : prev.y;
            bounds.stretch(borderX, borderY);
        }
    }
    //choose y-axis as independent
    else
    {
        while(prev.y != p.y)
        {
            bool headedDown = (prev.y < p.y);
            bool headedRight = (prev.x < p.x);

            prev.y = (prev.y < p.y) ? prev.y+1 : prev.y-1;
            prev.x += slopeXoverY;

            lines.push_back(new Pixel(prev, r, c));
            int borderY = (headedDown) ? prev.y+(2*r) : prev.y;
            int borderX = (headedRight) ? prev.x+(2*r) : prev.x;
            bounds.stretch(borderX, borderY);
        }
    }

    prev = sf::Vector2f(p);
    points.push_back(std::make_pair(p, r));
    return true;
}

int Squiggle::count()
{
    std::cout << "dots: " << lines.size() << " vs points: " << points.size() << std::endl;
    return lines.size();
}

Box& Squiggle::getBounds()
{
    return bounds;
}

Squiggle::Squiggle(std::fstream& file)
{
    optimized = false;

    unsigned int size = 0;
    file.read((char*)&size, sizeof(size));

    //where was the bounds again?
    int x = 0;
    file.read((char*)&x, sizeof(x));
    int y = 0;
    file.read((char*)&y, sizeof(y));
    bounds = Box(x, y);

    //load the color of the squiggle
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;
    file.read((char*)&r, sizeof(r));
    file.read((char*)&g, sizeof(g));
    file.read((char*)&b, sizeof(b));
    file.read((char*)&a, sizeof(a));
    Color ink(r, g, b, a);
    
    //rebuild the lines
    unsigned short localX = 0;
    unsigned short localY = 0;
    unsigned char radius = 0;
    for(unsigned int i = 0; i < size; i++)
    {
        //location
        file.read((char*)&localX, sizeof(localX));
        file.read((char*)&localY, sizeof(localY));
        //radius
        file.read((char*)&radius, sizeof(radius));
        sf::Vector2i location(localX+bounds.getX(), localY+bounds.getY());
        if(i == 0) //start initial point
        {
            this->prev = sf::Vector2f(location);
            lines.push_back(new Pixel(prev, radius, ink));
            bounds.stretch(prev.x+radius*2, prev.y+radius*2);
            points.push_back(std::make_pair(location, radius));
        }
        else //continuation points
            addPoint(location, radius, ink);
    }

    scalar = 1;
}

void Squiggle::save(std::fstream& file)
{
    unsigned int size = points.size();
    file.write((char*)&size, sizeof(size)); //to remember how many points are in this squiggle

    //save the global position
    int temp = bounds.getX();
    file.write((char*)&temp, sizeof(temp));
    temp = bounds.getY();
    file.write((char*)&temp, sizeof(temp));

    //save the color of the squiggle
    unsigned char r = lines.front()->getColor().GetR();
    unsigned char g = lines.front()->getColor().GetG();
    unsigned char b = lines.front()->getColor().GetB();
    unsigned char a = lines.front()->getColor().GetA();
    file.write((char*)&r, sizeof(r));
    file.write((char*)&g, sizeof(g));
    file.write((char*)&b, sizeof(b));
    file.write((char*)&a, sizeof(a));
    
    //save every recorded point
    unsigned short localX = 0;
    unsigned short localY = 0;
    unsigned char radius = 0;
    for(auto it = points.begin(); it != points.end(); it++)
    {
        auto& p = (*it);
        //location
        localX = (unsigned short)(p.first.x-bounds.getX());
        localY = (unsigned short)(p.first.y-bounds.getY());
        file.write((char*)&localX, sizeof(localX));
        file.write((char*)&localY, sizeof(localY));
        //radius
        radius = (unsigned char)p.second;
        file.write((char*)&radius, sizeof(radius));
    }
}