#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsResource.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

class CharacterControllerComponent : public Component {
    TYPE_DEFINITION(CharacterControllerComponent);
public:
    CharacterControllerComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    void create(TransformComponent& transform);
    bool isReady() override;
    void update(TransformComponent &transform, bool isSimulationEnabled) const;

    float m_height;
    float m_radius;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    physx::PxRigidDynamic *m_pxRigidActor;
};
