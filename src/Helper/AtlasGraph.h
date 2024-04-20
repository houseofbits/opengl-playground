#pragma once

#include "Console.h"
#include <iostream>
#include <string>

class AtlasNode
{
public:
    int parentIndex;
    int childIndices[4];
    bool isOccupied;
    unsigned int left;
    unsigned int top;
    unsigned int width;
    unsigned int height;
};

class AtlasGraph
{
public:
    unsigned int MIN_WIDTH;
    unsigned int MIN_HEIGHT;

    AtlasNode nodes[2000];
    unsigned int size;

    void generate(unsigned int width, unsigned int height, unsigned int minWidth = 128, unsigned int minHeight = 128)
    {
        MIN_WIDTH = minWidth;
        MIN_HEIGHT = minHeight;

        size = 0;

        AtlasNode node;
        node.parentIndex = -1;
        node.isOccupied = false;
        node.width = width;
        node.height = height;
        node.left = 0;
        node.top = 0;

        nodes[size] = node;

        size++;

        generateRecursive();
    }

    void generateRecursive(int parentIndex = 0)
    {
        AtlasNode &parent = nodes[parentIndex];

        if (parent.width / 2 < MIN_WIDTH || parent.height / 2 < MIN_HEIGHT)
        {
            parent.childIndices[0] = -1;
            parent.childIndices[1] = -1;
            parent.childIndices[2] = -1;
            parent.childIndices[3] = -1;

            return;
        }

        int index = 0;
        for (int w = 0; w < 2; w++)
        {
            for (int h = 0; h < 2; h++)
            {
                AtlasNode node;
                node.parentIndex = parentIndex;
                node.isOccupied = false;
                node.width = parent.width / 2;
                node.height = parent.height / 2;
                node.left = parent.left + (node.width * w);
                node.top = parent.top + (node.height * h);

                unsigned int nodeIndex = size;
                nodes[nodeIndex] = node;
                parent.childIndices[index] = nodeIndex;

                size++;
                index++;

                generateRecursive(nodeIndex);
            }
        }
    }

    int occupyFirstAvailable(unsigned int width, unsigned int height)
    {
        int index = findFirstAvailable(width, height);
        if (index >= 0)
        {
            nodes[index].isOccupied = true;
        }

        return index;
    }

    int findFirstAvailable(unsigned int width, unsigned int height, unsigned int nodeIndex = 0)
    {
        if (nodes[nodeIndex].width >= width * 2 && nodes[nodeIndex].height >= height * 2)
        {
            for (unsigned int i = 0; i < 4; i++)
            {
                if (nodes[nodeIndex].childIndices[i] >= 0)
                {
                    int index = findFirstAvailable(width, height, nodes[nodeIndex].childIndices[i]);
                    if (index >= 0)
                    {
                        return index;
                    }
                }
            }
        }

        return areChildrenOccupied(nodeIndex) ? -1 : nodeIndex;
    }

    bool areChildrenOccupied(unsigned int index)
    {
        if (!nodes[index].isOccupied)
        {
            for (unsigned int i = 0; i < 4; i++)
            {
                if (nodes[index].childIndices[i] >= 0)
                {
                    return areChildrenOccupied(nodes[index].childIndices[i]);
                }
            }
        }

        return nodes[index].isOccupied;
    }

    void clearAll(unsigned int index = 0)
    {
        nodes[index].isOccupied = false;

        for (unsigned int i = 0; i < 4; i++)
        {
            if (nodes[index].childIndices[i] >= 0)
            {
                clearAll(nodes[index].childIndices[i]);
            }
        }
    }

    void debug()
    {
        std::cout << "Size: " << size << std::endl;

        recursiveDebug("");
    }

    void recursiveDebug(std::string indent, int index = 0)
    {
        const AtlasNode &node = nodes[index];

        std::cout << FOREYEL << indent << index << ": " << FORECYN << node.width << "," << node.height << " " << node.left << "," << node.top << " ";

        std::string occupied = "false";
        if (node.isOccupied)
        {
            std::cout << BACKRED << FOREWHT << "true";
        }
        else
        {
            std::cout << FOREGRN << "false";
        }

        std::cout << RESETTEXT << std::endl;

        if (node.childIndices[0] >= 0)
        {
            recursiveDebug(indent + " ", node.childIndices[0]);
            recursiveDebug(indent + " ", node.childIndices[1]);
            recursiveDebug(indent + " ", node.childIndices[2]);
            recursiveDebug(indent + " ", node.childIndices[3]);
        }
    }
};
