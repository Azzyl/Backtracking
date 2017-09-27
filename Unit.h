#ifndef UNIT_H
#define UNIT_H
#include <string>
using namespace std;

//Class for all units except red hood
class Unit
{
    public:
        string name;
        int x,y;
        Unit()
        {
            name = "";
            x = 0;
            y = 0;
            active = false;
        }
        Unit(string name, int x, int y)
        {
            this->x = x;
            this->y = y;
            this->name = name;
            active = false;
        }
        bool active;
};

//Red hood class
class redHood
{
public:
    int x, y, ber;
    redHood()
    {
        ber = 6;
        x = 0;
        y = 0;
    }
};

class Coordinates
{
public:
    int x, y;
    Coordinates(int x,int y)
    {
        this->x = x;
        this->y = y;
    }
    Coordinates() { }
};

#endif /* UNIT_H */

