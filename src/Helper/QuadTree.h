#pragma once

class QuadTreeNode
{
public:
    int parentIndex;
    int childIndices[4];
    unsigned int left;
    unsigned int top;
    unsigned int size;
};

class QuadTree
{
private:
    unsigned int depth;
    int sizeIterator = 0;
    unsigned int numNodes;
    QuadTreeNode *nodes;

    void createNodesRecursive(int parentIndex, unsigned int depth);

public:
    void create(unsigned int depth);
};