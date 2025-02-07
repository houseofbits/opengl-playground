#pragma once
#include <string>

class EditorUISystem;
class BaseComponentEdit;
class Camera;
class Entity;

class TransformHelper {
public:
    explicit TransformHelper(EditorUISystem &uiSystem) : m_UISystem(&uiSystem) {
    }

    void processToolbar();

    void processGizmo(Camera &camera);

private:
    void handleEntitySelection(Entity&);

    void processTransformTargetDropdown(Entity&);

    void processTransformOperationDropdown();

    void processTransformSpaceDropdown();

    EditorUISystem *m_UISystem{nullptr};
    BaseComponentEdit *m_pSelectedComponentEdit{nullptr};
    int m_selectedEntityId{-1};
    int m_selectedTransformTargetIndex{-1};
    long m_selectedTransformOperation{0};
    long m_selectedTransformSpace{0};
};
