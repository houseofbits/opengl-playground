#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include <PhysX/PxPhysics.h>

class RigidBodyComponent : public Component {
    TYPE_DEFINITION(RigidBodyComponent);
public:
    RigidBodyComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;

    physx::PxRigidDynamic* m_pxRigidBody;
    physx::PxTransform m_pxInitialTransform;
    float m_density;
    glm::vec2 m_friction;
    float m_restitution;
};
