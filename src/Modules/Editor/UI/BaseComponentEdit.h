#pragma once

#include <string>
#include <glm/mat4x4.hpp>

class EditorUISystem;
class Entity;
class Component;

class BaseComponentEdit {
public:
    struct TransformOption {
        std::string name;
        bool isTranslationEnabled{false};
        bool isRotationEnabled{false};
        bool isScalingEnabled{false};
        long defaultOperation{0};
    };

    explicit BaseComponentEdit(EditorUISystem &editorSystem) : m_editorSystem(&editorSystem) {
    }

    virtual ~BaseComponentEdit() = default;

    virtual std::string getName() = 0;

    virtual void process(Entity &entity, EditorUISystem &system) {
    }

    virtual void processEditor(Entity &e, Component *c) {
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
