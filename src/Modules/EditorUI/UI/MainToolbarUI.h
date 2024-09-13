#pragma once

#include "../Events/EditorUIEvent.h"
class TransformComponent;
class EditorUISystem;

class MainToolbarUI {
public:
    explicit MainToolbarUI(EditorUISystem *);

    void process();

    bool m_isEditWindowVisible;
    long m_currentGizmoOperation;
    long m_currentGizmoMode;
    int m_renderShaderType;
    bool m_isSimulationEnabled;
    int m_selectedCameraComponentId;
    int m_previousSelectedCameraComponentId;

private:
    void processTransformTypeDropdown(TransformComponent *transform);
    void processTransformSpaceDropdown();
    void processViewMenu();
    void sendUIEvent(EditorUIEvent::Type type);
    void sendSaveEvent();
    void sendEditorStateEvent();
    void runSimulation();
    void stopSimulation();
    void activateCameras();

    EditorUISystem *m_EditorUISystem;
};
