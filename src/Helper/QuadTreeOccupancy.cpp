#include "QuadTreeOccupancy.h"
#include <iostream>

QuadTreeOccupancy::QuadTreeOccupancy() : occupancyMap()
{
}

QuadTreeOccupancy::~QuadTreeOccupancy()
{
}

bool QuadTreeOccupancy::isNodeOccupied(unsigned int nodeIndex)
{
    return occupancyMap[nodeIndex];
}

bool QuadTreeOccupancy::isBranchOccupied(QuadTree &tree, unsigned int nodeIndex)
{
    if (!occupancyMap[nodeIndex])
    {
        const QuadTreeNode &node = tree.getNode(nodeIndex);

        for (unsigned int i = 0; i < 4; i++)
        {
            if (node.childIndices[i] >= 0 && isBranchOccupied(tree, node.childIndices[i]))
            {
                return true;
            }
        }
    }

    return occupancyMap[nodeIndex];
}

int QuadTreeOccupancy::findFreeNodeOfSize(QuadTree &tree, unsigned int size, unsigned int nodeIndex)
{
    const QuadTreeNode &node = tree.getNode(nodeIndex);
    if (!occupancyMap[nodeIndex] && size >= node.size * 2)
    {
        for (unsigned int i = 0; i < 4; i++)
        {
            if (node.childIndices[i] >= 0)
            {
                int index = findFreeNodeOfSize(tree, size, node.childIndices[i]);
                if (index >= 0)
                {
                    return index;
                }
            }
        }
    }

    return isBranchOccupied(tree, nodeIndex) ? -1 : nodeIndex;
}

void QuadTreeOccupancy::setOccupied(QuadTree &tree, unsigned int nodeIndex, bool value)
{
    occupancyMap[nodeIndex] = value;
}

void QuadTreeOccupancy::clear()
{
    occupancyMap.clear();
}