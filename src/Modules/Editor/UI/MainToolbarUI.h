#pragma once

#include "../Events/EditorUIEvent.h"
class TransformComponent;
class EditorUISystem;

class MainToolbarUI {
public:
    explicit MainToolbarUI(EditorUISystem *);

    void process();

    int m_renderShaderType;
    bool m_showPhysicsShapesWireframe{true};
    bool m_showMaterialEditor{false};

    void selectCameraByIndex(unsigned int index) const;

private:
    void processViewMenu();
    void sendUIEvent(EditorUIEvent::Type type) const;
    void enableGameMode() const;
    void enableEditorMode() const;

    EditorUISystem *m_EditorUISystem;
};
