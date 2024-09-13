#include "QuadTree.h"
#include <assert.h>
#include <math.h>
#include <iostream>

QuadTree::QuadTree() : numNodes(0), nodes(nullptr), depth(0), sizeIterator(0)
{
}

QuadTree::~QuadTree()
{
}

void QuadTree::create(unsigned int treeDepth)
{

    depth = treeDepth;
    sizeIterator = 0;

    numNodes = 1;
    for (int i = 1; i < depth; i++)
    {
        numNodes += pow(4, i);
    }

    if (nodes != nullptr)
    {
        delete[] nodes;
    }

    nodes = new QuadTreeNode[numNodes];

    QuadTreeNode node;
    node.parentIndex = -1;
    node.size = 1;
    node.left = 0;
    node.top = 0;
    node.childIndices[0] = -1;
    node.childIndices[1] = -1;
    node.childIndices[2] = -1;
    node.childIndices[3] = -1;
    nodes[sizeIterator] = node;
    sizeIterator++;

    createNodesRecursive(0, 1);
}

void QuadTree::createNodesRecursive(int parentIndex, unsigned int currentDepth)
{
    if (currentDepth + 1 > depth)
    {
        return;
    }

    QuadTreeNode &parent = nodes[parentIndex];
    unsigned int i = 0;
    for (unsigned int x = 0; x < 2; x++)
    {
        for (unsigned int y = 0; y < 2; y++)
        {
            QuadTreeNode node;
            node.parentIndex = parentIndex;
            node.size = parent.size * 2;
            node.left = (parent.left * 2) + x;
            node.top = (parent.top * 2) + y;
            node.childIndices[0] = -1;
            node.childIndices[1] = -1;
            node.childIndices[2] = -1;
            node.childIndices[3] = -1;

            unsigned int nodeIndex = sizeIterator;
            nodes[nodeIndex] = node;

            parent.childIndices[i] = nodeIndex;

            sizeIterator++;
            i++;

            createNodesRecursive(nodeIndex, currentDepth + 1);
        }
    }
}

const QuadTreeNode &QuadTree::getNode(unsigned int index)
{
    assert(index < numNodes);

    return nodes[index];
}