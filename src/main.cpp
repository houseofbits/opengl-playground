#include "Applications/Editor/EditorApplication.h"
#include "Applications/Main/MainApplication.h"
#include "Renderer/Model/GLTFFileLoader.h"
#include "Renderer/Model/ModelConfigurationLoader.h"

int main(int argc, char *argv[]) {
    // std::string filename(argv[1]);
    std::string filename("ship.json");//filename(argv[2]); //"data/scenes/physics.json"

    // if (argc > 1 && std::string(argv[1]) == "-e") {
        EditorApplication app;
        app.initialize("data/scenes/" + filename, "data/entityMap.json");
        app.run();
    // } else {
    //     MainApplication app;
    //     app.initialize("data/scenes/" + filename, "data/entityMap.json");
    //     app.run();
    // }

    return 0;
}
