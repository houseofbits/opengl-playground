#include "Applications/Editor/EditorApplication.h"
// #include "Applications/Main/MainApplication.h"
// #include "Renderer/Model/GLTFFileLoader.h"
#include "Renderer/Model/ModelConfigurationLoader.h"
#include "Modules/Application/Helpers/Window.h"

int main(int argc, char *argv[]) {
    // // typesp<MainApplication, int>();
    //
    // type_name2<MainApplication, EditorApplication, EditorApplication, EditorApplication>();
    // return 0;

    // std::string filename(argv[1]);
    std::string filename("test.json"); //filename(argv[2]); //"data/scenes/physics.json"

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

//
// #include "Modules/Application/Helpers/Window.h"
// #include "Applications/Editor/EditorApplication.h"
//
// int main(int argc, char *argv[]) {
//   EditorApplication app;
//
//   std::string filename("test.json");
//   app.initialize("data/scenes/" + filename, "data/entityMap.json");
//
//   return 1;
// }

