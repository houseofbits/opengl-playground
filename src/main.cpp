#include "Include.h"
#include <list>
#include <typeindex>
#include <iostream>
#include "Renderer/TextureAtlasManager.h"

int main(int argc, char *argv[])
{
    // AtlasGraph graph;
    // graph.generate(2048, 2048);

    // for (int i = 0; i < 10; i++)
    // {
    //     int index = graph.occupyFirstAvailable(256, 256);
    //     std::cout << index << std::endl;
    // }
    // graph.clearAll();
    // graph.debug();

    TextureAtlasManager mgr;
    mgr.init();
    // mgr.printNodesWithRegions(1024);

    // Entry entry;
    // entry.run();
}