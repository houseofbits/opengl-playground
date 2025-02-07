#include "Applications/Editor/EditorApplication.h"
#include "Applications/Main/MainApplication.h"

int main(int argc, char *argv[]) {
    std::string filename("hinge-joint.json");//filename(argv[2]); //"data/scenes/physics.json"

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
