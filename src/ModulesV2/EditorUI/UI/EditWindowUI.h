#pragma once

#include <string>

class EditorUISystem;

class EditWindowUI {
public:
    explicit EditWindowUI(EditorUISystem *);

    void process();
    bool isTransformComponentSelected();

    int m_selectedEntity;
private:

    void processEntitiesList();
    void processEditLightComponent();
    void processEditTransformComponent();
    void processEditMeshComponent();
    void processEditCameraComponent();
    void sendEntityCreationEvent(std::string);
    void sendEntityRemovalEvent();

    EditorUISystem *m_EditorUISystem;
    std::string m_lightProjectorPath;
    std::string m_meshModelPath;
};
