#pragma once

#include <string>
#include <glm/mat4x4.hpp>

class EditorUISystem;
class Entity;
class Component;

class BaseComponentTransformEdit {
public:
    struct TransformOption {
        std::string name;
        bool isTranslationEnabled{false};
        bool isRotationEnabled{false};
        bool isScalingEnabled{false};
        long defaultOperation{0};
        bool isBoundsTransformEnabled{false};
        bool isTransformCenterGizmoVisible{true};
    };

    explicit BaseComponentTransformEdit(EditorUISystem &editorSystem) : m_editorSystem(&editorSystem) {
    }

    virtual ~BaseComponentTransformEdit() = default;

    virtual void handleEntitySelection(Entity &e, Component *c) {

    }

    virtual int getNumTransformTargets() {
        return 0;
    }

    virtual TransformOption getTransformTargetOptions(int index) {
        return {};
    }

    virtual glm::mat4 getWorldSpaceTransform(int index) {
        return {};
    }

    virtual void setWorldSpaceTransform(glm::mat4 m, int index) const {
    }

protected:
    EditorUISystem *m_editorSystem{nullptr};
};
