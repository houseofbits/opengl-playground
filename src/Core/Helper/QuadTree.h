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
    int sizeIterator;
    unsigned int numNodes;
    QuadTreeNode *nodes;

    void createNodesRecursive(int parentIndex, unsigned int depth);

public:
    QuadTree();
    ~QuadTree();

    void create(unsigned int depth);
    const QuadTreeNode &getNode(unsigned int index);
    unsigned int getNumNodes()
    {
        return numNodes;
    }
    unsigned int getDepth()
    {
        return depth;
    }
};