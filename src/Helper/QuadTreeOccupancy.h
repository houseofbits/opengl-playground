#pragma once

#include <map>
#include "QuadTree.h"

class QuadTreeOccupancy
{
private:
    std::map<unsigned int, bool> occupancyMap;

public:
    QuadTreeOccupancy();
    ~QuadTreeOccupancy();

    bool isBranchOccupied(QuadTree &tree, unsigned int nodeIndex);
    bool isNodeOccupied(unsigned int nodeIndex);
    int findFreeNodeOfSize(QuadTree &tree, unsigned int size, unsigned int nodeIndex = 0);
    void setOccupied(QuadTree &tree, unsigned int nodeIndex, bool value);
};