#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsResource.h"

class CharacterControllerComponent : public Component {
    TYPE_DEFINITION(CharacterControllerComponent);
public:
    CharacterControllerComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    void create(TransformComponent& transform);
    bool isReady() override;
    void update(TransformComponent &transform, bool isSimulationEnabled);
    void move(glm::vec3) const;

    float m_height;
    float m_radius;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    btRigidBody *m_rigidBody;
    bool m_isOnGround;
    RayCastResult m_groundRayCast;
    float m_stepTolerance;      //aka. knee-height

private:
    bool castRayForGroundReference(const glm::vec3 &point);
    void updateGroundSpring(const glm::vec3& kneePoint);
};
