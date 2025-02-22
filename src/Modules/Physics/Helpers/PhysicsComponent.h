#pragma once

#include "../../../Core/API.h"

class PhysicsComponent : public Component {
public:
    PhysicsComponent() : Component() {
    }

    ~PhysicsComponent() override = default;

    [[nodiscard]] bool isPhysicsCreated() const {
        return !m_shouldCreatePhysics;
    }

    [[nodiscard]] bool shouldCreatePhysics() const {
        return m_shouldCreatePhysics;
    }

    void setPhysicsCreated(bool created) {
        m_shouldCreatePhysics = !created;
    }

    virtual bool isReadyToCreate(EntityContext &ctx) const {
        return true;
    }

    virtual void createPhysics(EntityContext &ctx) {
    }

private:
    bool m_shouldCreatePhysics{true};
};
