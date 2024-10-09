#pragma once


#include <glm/fwd.hpp>

class ComponentTransformEdit {
public:
    ComponentTransformEdit() = default;

    virtual ~ComponentTransformEdit() = default;

    virtual glm::mat4 getEditorTransform() = 0;

    virtual void setFromEditorTransform(const glm::mat4 &) = 0;
};
