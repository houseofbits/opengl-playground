#include "Applications/Main/MainApplication.h"

int main(int argc, char *argv[]) {
    MainApplication app;
    app.initialize("data/entityMap.json");
    app.loadEntitiesFromFile("data/scenes/test-bullet.json");
    app.run();

    return 0;
}