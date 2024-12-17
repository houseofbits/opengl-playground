#pragma once


#include <glm/fwd.hpp>

class ComponentTransformEdit {
public:
    ComponentTransformEdit() = default;

    virtual ~ComponentTransformEdit() = default;

    virtual glm::mat4 getWorldTransform() = 0;

    virtual void setWorldTransform(const glm::mat4 &) = 0;
};
