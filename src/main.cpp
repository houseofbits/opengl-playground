#include "Applications/Editor/EditorApplication.h"
#include "Applications/Main/MainApplication.h"

int main(int argc, char *argv[]) {
    if (argc > 1 && std::string(argv[1]) == "-e") {
        EditorApplication app;
        app.initialize("data/entityMap.json");
        app.run();
    } else {
        MainApplication app;
        app.initialize("data/entityMap.json");
        app.run();
    }

    return 0;
}

// #include <fbxsdk.h>
// #include <iostream>
// #include <string>
// #include "SourceLibs/FBXInfo/FBXInfo.h"
//
// bool InitializeSdkObjects(FbxManager *&manager, FbxScene *&scene) {
//     manager = FbxManager::Create();
//     if (!manager) {
//         std::cerr << "Error: Unable to create FBX Manager!" << std::endl;
//         return false;
//     }
//
//     FbxIOSettings *ios = FbxIOSettings::Create(manager, IOSROOT);
//     manager->SetIOSettings(ios);
//
//     scene = FbxScene::Create(manager, "Scene");
//     if (!scene) {
//         std::cerr << "Error: Unable to create FBX Scene!" << std::endl;
//         return false;
//     }
//
//     return true;
// }
//
// // Function to load an FBX file
// bool LoadFBXFile(FbxManager *manager, FbxScene *scene, const std::string &filePath) {
//     FbxImporter *importer = FbxImporter::Create(manager, "");
//
//     if (!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings())) {
//         std::cerr << "Error: Unable to initialize importer for " << filePath << std::endl;
//         std::cerr << "Error: " << importer->GetStatus().GetErrorString() << std::endl;
//         importer->Destroy();
//         return false;
//     }
//
//     if (!importer->Import(scene)) {
//         std::cerr << "Error: Unable to import FBX file!" << std::endl;
//         importer->Destroy();
//         return false;
//     }
//
//     importer->Destroy();
//     return true;
// }
//
// int main(int argc, char **argv) {
//     std::string filePath = "data/models/test-models.FBX";
//
//     FbxManager *manager = nullptr;
//     FbxScene *scene = nullptr;
//
//     if (!InitializeSdkObjects(manager, scene)) {
//         return 1;
//     }
//
//     if (!LoadFBXFile(manager, scene, filePath)) {
//         manager->Destroy();
//         return 1;
//     }
//
//     FBXInfo::DisplayContent(scene);
//
//     manager->Destroy();
//
//     return 0;
// }
