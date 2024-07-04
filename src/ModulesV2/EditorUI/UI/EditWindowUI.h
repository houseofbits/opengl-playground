#pragma once

#include <string>

class EditorUISystem;

class EditWindowUI {
public:
    explicit EditWindowUI(EditorUISystem *);

    void process();
    bool isTransformComponentSelected();

    int m_selectedEntity;
    bool m_isBoundsTransformAllowed;
private:

    void processEntitiesList();
    void processEditLightComponent();
    void processEditTransformComponent();
    void processEditMeshComponent();
    void processEditCameraComponent();
    void processEditProbeComponent();
    void sendEntityCreationEvent(std::string, std::string);
    void sendEntityRemovalEvent();

    EditorUISystem *m_EditorUISystem;
    std::string m_lightProjectorPath;
    std::string m_meshModelPath;
    int m_selectedEntityCreationType;
};
