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
    std::string m_meshMaterialPath;
    int m_selectedEntityCreationType;
    bool m_isLightEntitiesListed;
    bool m_isStaticMeshEntitiesListed;
    bool m_isCameraEntitiesListed;
    bool m_isProbeEntitiesListed;
    bool m_isNewMaterialPromptVisible;
};
