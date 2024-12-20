#include "Applications/Editor/EditorApplication.h"
#include "Applications/Main/MainApplication.h"

int main(int argc, char *argv[]) {
    if (argc > 1 && std::string(argv[1]) == "-e") {
        EditorApplication app;
        app.initialize("data/entityMap.json");
        app.loadEntitiesFromFile("data/scenes/physics.json");
        app.run();
    } else {
        MainApplication app;
        app.initialize("data/entityMap.json");
        app.loadEntitiesFromFile("data/scenes/physics.json");
        app.run();
    }

    return 0;
}
