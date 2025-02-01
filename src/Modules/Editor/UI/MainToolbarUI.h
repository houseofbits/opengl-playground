#pragma once

#include "../Events/EditorUIEvent.h"
class TransformComponent;
class EditorUISystem;

class MainToolbarUI {
public:
    explicit MainToolbarUI(EditorUISystem *);

    void process();

    int m_renderShaderType;

private:
    void processViewMenu();
    void sendUIEvent(EditorUIEvent::Type type) const;
    void enableGameMode() const;
    void enableEditorMode() const;

    EditorUISystem *m_EditorUISystem;
};
