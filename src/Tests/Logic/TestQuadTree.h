#pragma once

#include <assert.h>
#include "iostream"
#include "../../Helper/Console.h"
#include "../../Helper/QuadTree.h"
#include "../../Helper/QuadTreeOccupancy.h"

class TestQuadTree
{
public:
    static void run()
    {
        testCreate();
        testIsBranchOccupied();
        testFindFreeNodeOfSize();
    }

    static void testCreate()
    {
        QuadTree qt;

        qt.create(6);

        assert(qt.getNumNodes() == 1365);

        assert(qt.getNode(1).size == 2);
        assert(qt.getNode(2).size == 4);
        assert(qt.getNode(3).size == 8);
        assert(qt.getNode(4).size == 16);
        assert(qt.getNode(5).size == 32);
        assert(qt.getNode(6).size == 32);
        assert(qt.getNode(7).size == 32);
        assert(qt.getNode(8).size == 32);

        assert(qt.getNode(1).left == 0);
        assert(qt.getNode(2).left == 0);
        assert(qt.getNode(3).left == 0);
        assert(qt.getNode(4).left == 0);
        assert(qt.getNode(5).left == 0);
        assert(qt.getNode(6).left == 0);
        assert(qt.getNode(7).left == 1);
        assert(qt.getNode(8).left == 1);

        assert(qt.getNode(1).top == 0);
        assert(qt.getNode(2).top == 0);
        assert(qt.getNode(3).top == 0);
        assert(qt.getNode(4).top == 0);
        assert(qt.getNode(5).top == 0);
        assert(qt.getNode(6).top == 1);
        assert(qt.getNode(7).top == 0);
        assert(qt.getNode(8).top == 1);

        printDebugQTStructure(qt);
    }

    static void testIsBranchOccupied()
    {
        QuadTree qt;

        qt.create(6);

        QuadTreeOccupancy qtOccupancy;

        qtOccupancy.setOccupied(qt, 7, true);

        assert(qtOccupancy.isBranchOccupied(qt, 0) == true);
        assert(qtOccupancy.isBranchOccupied(qt, 1) == true);
        assert(qtOccupancy.isBranchOccupied(qt, 2) == true);
        assert(qtOccupancy.isBranchOccupied(qt, 3) == true);
        assert(qtOccupancy.isBranchOccupied(qt, 4) == true);
        assert(qtOccupancy.isBranchOccupied(qt, 7) == true);

        assert(qtOccupancy.isBranchOccupied(qt, 5) == false);
        assert(qtOccupancy.isBranchOccupied(qt, 9) == false);

        qtOccupancy.setOccupied(qt, 7, false);

        assert(qtOccupancy.isBranchOccupied(qt, 0) == false);
    }

    static void testFindFreeNodeOfSize()
    {
        QuadTree qt;

        qt.create(6);

        QuadTreeOccupancy qtOccupancy;

        qtOccupancy.setOccupied(qt, 7, true);

        unsigned int index = qtOccupancy.findFreeNodeOfSize(qt, 32);

        assert(qtOccupancy.findFreeNodeOfSize(qt, 32) == 5);

        qtOccupancy.setOccupied(qt, 5, true);

        assert(qtOccupancy.findFreeNodeOfSize(qt, 32) == 6);

        qtOccupancy.setOccupied(qt, 6, true);

        assert(qtOccupancy.findFreeNodeOfSize(qt, 32) == 8);

        qtOccupancy.setOccupied(qt, 8, true);

        assert(qtOccupancy.findFreeNodeOfSize(qt, 32) == 10);

        qtOccupancy.setOccupied(qt, 10, true);

        assert(qtOccupancy.findFreeNodeOfSize(qt, 16) == 14);
    }

    static void printDebugQTStructure(QuadTree &tree, unsigned int nodeIndex = 0, std::string indent = "")
    {
        const QuadTreeNode &node = tree.getNode(nodeIndex);

        std::cout
            << indent
            << FOREWHT
            << nodeIndex
            << ": "
            << FOREGRN
            << node.size
            << " "
            << node.top
            << ", "
            << node.top
            << RESETTEXT
            << std::endl;

        for (unsigned int i = 0; i < 4; i++)
        {
            if (node.childIndices[i] > 0)
            {
                printDebugQTStructure(tree, node.childIndices[i], indent + " ");
            }
        }
    }

    static void printDebugQTStructureWithOccupancy(QuadTree &tree, QuadTreeOccupancy &occupancy, unsigned int nodeIndex = 0, std::string indent = "")
    {
        const QuadTreeNode &node = tree.getNode(nodeIndex);

        std::cout
            << indent
            << FOREWHT
            << nodeIndex
            << ": "
            << FOREGRN
            << node.size
            << " "
            << node.top
            << ", "
            << node.top;

        if (occupancy.isBranchOccupied(tree, nodeIndex))
        {
            if (occupancy.isNodeOccupied(nodeIndex))
            {
                std::cout << " " << BACKRED << FOREBLK << "OCCUPIED";
            }
            else
            {
                std::cout << " " << BACKYEL << FOREBLK << "OCCUPIED";
            }
        }

        std::cout << RESETTEXT << std::endl;

        for (unsigned int i = 0; i < 4; i++)
        {
            if (node.childIndices[i] > 0)
            {
                printDebugQTStructureWithOccupancy(tree, occupancy, node.childIndices[i], indent + " ");
            }
        }
    }
};