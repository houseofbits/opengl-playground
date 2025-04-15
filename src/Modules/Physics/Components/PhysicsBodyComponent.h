#pragma once

#include "PhysicsShapeComponent.h"
#include "../../../Core/API.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/PhysicsMeshResource.h"
#include "../Resources/PhysicsResource.h"
#include "../Helpers/PhysicsComponent.h"

class PhysicsBodyComponent : public PhysicsComponent {
    TYPE_DEFINITION(PhysicsBodyComponent);

    static constexpr glm::vec3 STATIC_COLOR = glm::vec3(0.5, 0.5, 0.5);
    static constexpr glm::vec3 DYNAMIC_COLOR = glm::vec3(0.0, 1.0, 0.0);
    static constexpr glm::vec3 STATIC_SENSOR_COLOR = glm::vec3(1, 0.0, 0.0);
    static constexpr glm::vec3 DYNAMIC_SENSOR_COLOR = glm::vec3(1, 0.5, 0.0);

public:
    inline static const std::string DAMPING_KEY = "damping";
    inline static const std::string TYPE_KEY = "type";
    inline static const std::string RESTITUTION_KEY = "restitution";
    inline static const std::string FRICTION_KEY = "friction";
    inline static const std::string MASS_KEY = "mass";
    inline static const std::string SENSOR_KEY = "isSensor";

    enum BodyType {
        BODY_TYPE_STATIC,
        BODY_TYPE_DYNAMIC,
    };

    PhysicsBodyComponent();

    ~PhysicsBodyComponent() override;

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void createPhysics(EntityContext &ctx) override;

    bool isReadyToCreate(EntityContext &ctx) const override;

    void update(TransformComponent &transform, bool isSimulationEnabled);

    void wakeUp();

    const JPH::Body *getReadableBody();

    [[nodiscard]] bool isValid() const;

    BodyType m_BodyType;
    bool m_isSensor;
    glm::vec2 m_friction;
    glm::vec2 m_damping;
    float m_restitution;
    float m_mass;
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    JPH::BodyID m_physicsBodyId;

private:
    void release();
};
