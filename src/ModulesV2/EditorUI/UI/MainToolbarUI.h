#pragma once

class TransformComponent;
class EditorUISystem;

class MainToolbarUI {
public:
    explicit MainToolbarUI(EditorUISystem *);

    void process();

    bool m_isEditWindowVisible;
    long m_currentGizmoOperation;
    long m_currentGizmoMode;

private:
    void processTransformTypeDropdown(TransformComponent *transform);
    void processTransformSpaceDropdown();
    void sendUIEvent(EditorUIEvent::Type type);
    void sendSaveEvent();
    void sendEditorStateEvent();

    EditorUISystem *m_EditorUISystem;
};
