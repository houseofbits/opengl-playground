#pragma once

#include "../Events/EditorUIEvent.h"
class TransformComponent;
class EditorUISystem;

class MainToolbarUI {
public:
    explicit MainToolbarUI(EditorUISystem *);

    void process();

    bool m_isEditWindowVisible;
    int m_renderShaderType;
    bool m_isSimulationEnabled;

private:
    void processViewMenu();
    void sendUIEvent(EditorUIEvent::Type type);
    void sendSaveEvent();
    void sendEditorStateEvent();
    void runSimulation();
    void stopSimulation();

    EditorUISystem *m_EditorUISystem;
};
