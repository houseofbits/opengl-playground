#include "Applications/Editor/EditorApplication.h"
#include "Applications/Main/MainApplication.h"
#include "Renderer/Model/GLTFFileLoader.h"
#include "Renderer/Model/ModelConfigurationLoader.h"

// template <typename Last>
// void type_name2 () {
//     std::cout << "Type: " << typeid(Last).name() << std::endl;
// }
//
// template <typename First, typename Second, typename ...Rest>
// void type_name2 () {
//     type_name2<First>();
//     type_name2<Second, Rest...>();
// }

int main(int argc, char *argv[]) {
    // // typesp<MainApplication, int>();
    //
    // type_name2<MainApplication, EditorApplication, EditorApplication, EditorApplication>();
    // return 0;

    // std::string filename(argv[1]);
    std::string filename("tower.json"); //filename(argv[2]); //"data/scenes/physics.json"

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
