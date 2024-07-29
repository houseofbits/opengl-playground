#include "Include.h"
#include "ApplicationsV2/Main/MainApplication.h"

int main(int argc, char *argv[]) {
    //        Entry entry;
    //        entry.run();

    MainApplication app;
    app.initialize("data/entityMap.json");
    app.loadEntitiesFromFile("data/scenes/test.json");
    app.run();
}