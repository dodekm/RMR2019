#pragma once

#include "points.h"
#include <vector>

namespace map_loader
{

typedef struct
{
    int numofpoints;
    std::vector<Point> points;
}TMapObject;

typedef struct
{
   TMapObject wall;
   int numofObjects;
   std::vector<TMapObject> obstacle;
}TMapArea;

    
    void load_map(char *filename,TMapArea &mapss);
};

