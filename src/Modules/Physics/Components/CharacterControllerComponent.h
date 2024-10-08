#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsResource.h"
#include <Jolt/Physics/Character/Character.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>

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
    void move(glm::vec3);
    [[nodiscard]] bool isCreated() const;
    bool rayCast(glm::vec3 position, glm::vec3 direction, PhysicsRayCastResult& result);
    [[nodiscard]] glm::vec3 getVelocity() const;
    void setLookingDirection(glm::vec3& direction);

    float m_height;
    float m_radius;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isOnGround;
    float m_stepTolerance;      //aka. knee-height
    JPH::Body*  m_physicsBody;
    bool m_haveGroundDetected;
    float m_minDistanceToGround;
    glm::vec3 m_lookingDirection;

private:
    void updateMove();
    void castRayForGroundReference(const glm::vec3 &point);
    void updateGroundSpring(const glm::vec3 &kneePosition);

    float m_groundSpringForce;
    float m_groundSpringDamping;
    float m_freeFallForce;

    bool m_doMove;
    glm::vec3 m_movementDirection;
};
