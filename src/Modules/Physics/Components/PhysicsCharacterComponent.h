#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsResource.h"
#include <Jolt/Physics/Character/Character.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include "../Helpers/PhysicsComponent.h"

class PhysicsCharacterComponent : public PhysicsComponent {
TYPE_DEFINITION(PhysicsCharacterComponent);
public:
    PhysicsCharacterComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void create(TransformComponent &transform);

    void update(TransformComponent &transform, bool isSimulationEnabled);

    void move(glm::vec3);

    [[nodiscard]] bool isCreated() const;

    bool rayCast(glm::vec3 position, glm::vec3 direction, PhysicsRayCastResult &result, bool excludeAllSensors = true);

    [[nodiscard]] glm::vec3 getVelocity() const;

    void setMoveDirection(const glm::vec3 &direction);

    void createPhysics(EntityContext &ctx) override;

    bool isReadyToCreate(EntityContext &ctx) const override;

    float m_height;
    float m_radius;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isOnGround;
    float m_stepTolerance;      //aka. knee-height
    JPH::Body *m_physicsBody;
    bool m_haveGroundDetected;
    float m_minDistanceToGround;
    glm::vec3 m_moveDirection;

private:
    void updateMove();

    void castRayForGroundReference(const glm::vec3 &point);

    void updateGroundSpring(const glm::vec3 &kneePosition);

    std::optional<float> rayCastFeet(glm::vec3 position, glm::vec3 direction, bool doCheckGroundMovement = false);

    float m_groundSpringForce;
    float m_groundSpringDamping;
    float m_freeFallForce;

    bool m_doMove;
    glm::vec3 m_movementDirection;

    bool m_isGroundMoving;
    JPH::Vec3 m_groundMovementVelocity;
};
