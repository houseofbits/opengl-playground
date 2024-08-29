#include "Applications/Main/MainApplication.h"
#include "Include.h"

int main(int argc, char *argv[]) {
    //        Entry entry;
    //        entry.run();

    MainApplication app;
    app.initialize("data/entityMap.json");
    app.loadEntitiesFromFile("data/scenes/test-char.json");
    app.run();
}