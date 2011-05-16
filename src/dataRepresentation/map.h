#ifndef MAP_H
#define MAP_H
#include <vector>

class CityPosition{
public:
    int x;
    int y;
};

struct Road {
    int a;
    int b;
};



class Map
{
public:
    Map();

private:
    vector<CityPosition> cities;
    vector<Road> roads;

};

#endif // MAP_H
