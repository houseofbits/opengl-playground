#pragma once

#include "../../../Core/API.h"
#include "../Resources/PhysicsResource.h"
#include "PhysicsBodyComponent.h"

class PhysicsJointComponent : public Component  {
    TYPE_DEFINITION(PhysicsJointComponent);
public:
    inline static const std::string ENTITY_KEY = "targetEntity";
    inline static const std::string ARE_LIMITS_ENABLED_KEY = "enableLimits";
    inline static const std::string ARE_DRIVE_ENABLED_KEY = "enableDrive";
    inline static const std::string AXIS_KEY = "axis";
    inline static const std::string ATTACHMENT_A_KEY = "attachmentA";
    inline static const std::string ATTACHMENT_B_KEY = "attachmentB";
    inline static const std::string LIMITS_KEY = "limits";

    PhysicsJointComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    bool isReady() override;
    void create(PhysicsBodyComponent& bodyA, PhysicsBodyComponent& bodyB);
    void release();
    void attach(std::string entityName);
    void activate();
    void update();

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    std::string m_targetEntityName;
//    physx::PxRevoluteJoint* m_pxJoint;
    bool m_areLimitsEnabled;
    bool m_areDriveEnabled;
    glm::vec2 m_angularLimits;
    glm::vec3 m_axis;
    glm::vec3 m_localAttachmentA;
    glm::vec3 m_localAttachmentB;


    enum MovementState {
        UNKNOWN,
        CLOSED,
        OPEN,
        CLOSING,
        OPENING,
    };

    MovementState m_moveState;

};
