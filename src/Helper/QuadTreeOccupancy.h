#pragma once

#include <map>
#include "QuadTree.h"

class QuadTreeOccupancy
{
private:
    std::map<unsigned int, bool> occupancyMap;

public:
    void isBranchOccupied(const QuadTree &tree, unsigned int nodeIndex);
    unsigned int findFirstUnoccupiedNodeAtDepth(const QuadTree &tree, unsigned int depth);
    void setOccupied(const QuadTree &tree, unsigned int nodeIndex);
};