#pragma once

#include <glm/mat4x4.hpp>

#include "../../../Core/Reflection/Identity.h"

class WireframeRenderer;
class EditorUISystem;
class BaseComponentTransformEdit;
class Camera;
class Entity;

class TransformEdit {
public:
    explicit TransformEdit(EditorUISystem &uiSystem) : m_UISystem(&uiSystem) {
    }

    void processToolbar();

    void processGizmo(Camera &camera);

    void processDebugHelpers(WireframeRenderer &renderer) const;

    void handleEntitySelection(Entity &);

    void handleEntityDeselection();

private:
    void processTransformComponentDropdown(Entity &);

    void processTransformTargetDropdown(Entity &);

    void processTransformOperationDropdown();

    void processTransformSpaceDropdown();

    EditorUISystem *m_UISystem{nullptr};
    BaseComponentTransformEdit *m_pSelectedComponentEdit{nullptr};

    Identity::Type m_selectedComponentId{0};
    // int m_selectedEntityId{-1};
    int m_selectedTransformTargetIndex{-1};
    long m_selectedTransformOperation{0};
    long m_selectedTransformSpace{0};
    glm::mat4 m_transform{1.0};
};
