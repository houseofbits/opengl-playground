#pragma once

#include <glm/vec3.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Constraints/FixedConstraint.h>
#include <Jolt/Physics/Constraints/HingeConstraint.h>
#include <Jolt/Physics/Constraints/SliderConstraint.h>
#include "../../../../Core/Reflection/Identity.h"
#include "../Layers.h"
#include "../PhysicsTypeCast.h"
#include "../PhysicsUserData.h"
#include "../../../Common/Components/TransformComponent.h"
#include "../../Components/PhysicsBodyComponent.h"

class PhysicsBuilder {
    class PhysicsBodyBuilder {
        JPH::BodyInterface *m_bodyInterface = nullptr;
        Identity::Type m_entityId = 0;
        JPH::PhysicsMaterial *m_material = nullptr;
        JPH::BodyCreationSettings m_settings;

        [[nodiscard]] JPH::Body *createInner() const {
            const auto physicsBody = m_bodyInterface->CreateBody(m_settings);

            if (m_entityId != 0) {
                auto *userData = new PhysicsUserData(m_entityId);
                physicsBody->SetUserData(reinterpret_cast<unsigned long>(userData));
            }

            m_bodyInterface->AddBody(physicsBody->GetID(), JPH::EActivation::Activate);

            return physicsBody;
        }

    public:
        explicit PhysicsBodyBuilder(JPH::BodyInterface &bodyInterface): m_bodyInterface(&bodyInterface), m_settings() {
        }

        PhysicsBodyBuilder &setEntityReference(const Identity::Type entityId) {
            m_entityId = entityId;

            return *this;
        }

        PhysicsBodyBuilder &setDebugColor(const glm::vec3 color) {
            m_material = new JPH::PhysicsMaterialSimple(
                "Material", JPH::Color(static_cast<unsigned short>(255.0f * color.x),
                                       static_cast<unsigned short>(255.0f * color.y),
                                       static_cast<unsigned short>(255.0f * color.z)));

            return *this;
        }

        PhysicsBodyBuilder &setTransform(const glm::vec3 position, const glm::quat rotation) {
            m_settings.mPosition = PhysicsTypeCast::glmToJPH(position);
            m_settings.mRotation = PhysicsTypeCast::glmToJPH(rotation);

            return *this;
        }

        PhysicsBodyBuilder &setTransform(TransformComponent &transform) {
            m_settings.mPosition = PhysicsTypeCast::glmToJPH(transform.getWorldPosition());
            m_settings.mRotation = PhysicsTypeCast::glmToJPH(transform.getRotation());

            return *this;
        }

        PhysicsBodyBuilder &addBoxShape(glm::vec3 size) {
            m_settings.SetShape(new JPH::BoxShape({
                                                      size.x * 0.5f,
                                                      size.y * 0.5f,
                                                      size.z * 0.5f
                                                  }, JPH::cDefaultConvexRadius, m_material));

            return *this;
        }

        PhysicsBodyBuilder &setMass(const float mass) {
            m_settings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
            m_settings.mMassPropertiesOverride.mMass = mass;

            return *this;
        }

        PhysicsBodyBuilder &setDamping(const float linear, const float angular) {
            m_settings.mLinearDamping = linear;
            m_settings.mAngularDamping = angular;

            return *this;
        }

        PhysicsBodyBuilder &addTriangleMeshShape(const JPH::TriangleList &triangleList) {
            JPH::PhysicsMaterialList materials;
            materials.push_back(m_material);
            auto shapeSettings = new JPH::MeshShapeSettings(triangleList,
                                                            std::move(materials));

            m_settings.SetShapeSettings(shapeSettings);

            return *this;
        }

        PhysicsBodyBuilder &addConvexMeshShape(const JPH::Array<JPH::Vec3> &vertices) {
            auto shapeSettings = new JPH::ConvexHullShapeSettings(vertices,
                                                                  JPH::cDefaultConvexRadius, m_material);

            m_settings.SetShapeSettings(shapeSettings);

            return *this;
        }

        JPH::Body *createDynamic() {
            m_settings.mMotionType = JPH::EMotionType::Dynamic;
            m_settings.mObjectLayer = Layers::MOVING;

            return createInner();
        }

        JPH::Body *createStatic() {
            m_settings.mMotionType = JPH::EMotionType::Static;
            m_settings.mObjectLayer = Layers::NON_MOVING;

            return createInner();
        }

        JPH::Body *createStaticSensor() {
            m_settings.mMotionType = JPH::EMotionType::Static;
            m_settings.mObjectLayer = Layers::SENSOR;
            m_settings.mIsSensor = true;

            return createInner();
        }
    };

    class PhysicsJointBuilder {
        JPH::PhysicsSystem *m_physicsSystem = nullptr;
        JPH::Body *m_body1 = nullptr;
        JPH::Body *m_body2 = nullptr;
        glm::vec2 m_limits;
        bool m_isLimitsEnabled = false;
        bool m_isMotorSettingsEnabled = false;
        float m_motorFrequency = 0;
        float m_motorDamping = 0;
        float m_motorForceLimit = FLT_MAX;
        glm::vec3 m_axis1{0.0f};
        glm::vec3 m_axis2{0.0f};
        glm::vec3 m_point1{0.0f};
        glm::vec3 m_point2{0.0f};

    public:
        explicit PhysicsJointBuilder(JPH::PhysicsSystem &physicsSystem): m_physicsSystem(&physicsSystem), m_limits() {
        }

        PhysicsJointBuilder &setBodies(JPH::Body &body1, JPH::Body &body2) {
            m_body1 = &body1;
            m_body2 = &body2;

            return *this;
        }

        PhysicsJointBuilder &setBodies(const PhysicsBodyComponent &body1, const PhysicsBodyComponent &body2) {
            m_body1 = body1.m_physicsBody;
            m_body2 = body2.m_physicsBody;

            return *this;
        }

        PhysicsJointBuilder &setLimits(const glm::vec2 limits) {
            m_limits = limits;
            m_isLimitsEnabled = true;

            return *this;
        }

        PhysicsJointBuilder &setPoints(const glm::vec3 point1, const glm::vec3 point2) {
            m_point1 = point1;
            m_point2 = point2;

            return *this;
        }

        PhysicsJointBuilder &setAxis(const glm::vec3 axis1, const glm::vec3 axis2) {
            m_axis1 = axis1;
            m_axis2 = axis2;

            return *this;
        }

        PhysicsJointBuilder &setMotorSettings(const float frequency, const float damping) {
            m_motorFrequency = frequency;
            m_motorDamping = damping;
            m_isMotorSettingsEnabled = true;

            return *this;
        }

        PhysicsJointBuilder &setMotorForceLimit(const float limit) {
            m_motorForceLimit = limit;
            m_isMotorSettingsEnabled = true;

            return *this;
        }

        [[nodiscard]] JPH::FixedConstraint *createFixedConstraint() const {
            JPH::FixedConstraintSettings settings;
            settings.mAutoDetectPoint = true;
            auto joint = dynamic_cast<JPH::FixedConstraint *>(settings.Create(
                *m_body1, *m_body2));

            m_physicsSystem->AddConstraint(joint);

            return joint;
        }

        [[nodiscard]] JPH::SliderConstraint *createSliderConstraint(const glm::vec3 axis) const {
            JPH::SliderConstraintSettings settings;
            settings.mAutoDetectPoint = true;
            settings.SetSliderAxis(PhysicsTypeCast::glmToJPH(axis));
            if (m_isLimitsEnabled) {
                settings.mLimitsMin = m_limits.x;
                settings.mLimitsMax = m_limits.y;
            }

            auto joint = dynamic_cast<JPH::SliderConstraint *>(settings.Create(
                *m_body1, *m_body2));

            m_physicsSystem->AddConstraint(joint);

            if (m_isMotorSettingsEnabled) {
                JPH::MotorSettings &motor_settings = joint->GetMotorSettings();
                motor_settings.mSpringSettings.mFrequency = m_motorFrequency;
                motor_settings.mSpringSettings.mDamping = m_motorDamping;
                motor_settings.SetForceLimit(m_motorForceLimit);
            }

            return joint;
        }

        [[nodiscard]] JPH::HingeConstraint *createHingeConstraint() const {
            JPH::HingeConstraintSettings settings;
            settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM;
            settings.mPoint1 = PhysicsTypeCast::glmToJPH(m_point1);
            settings.mPoint2 = PhysicsTypeCast::glmToJPH(m_point2);
            settings.mHingeAxis1 = PhysicsTypeCast::glmToJPH(m_axis1);
            settings.mHingeAxis2 = PhysicsTypeCast::glmToJPH(m_axis2);
            settings.mNormalAxis1 = JPH::Vec3::sAxisX();
            settings.mNormalAxis2 = JPH::Vec3::sAxisX();

            if (m_isLimitsEnabled) {
                settings.mLimitsMin = m_limits.x;
                settings.mLimitsMax = m_limits.y;
            }

            auto joint = dynamic_cast<JPH::HingeConstraint *>(settings.Create(*m_body1, *m_body2));

            m_physicsSystem->AddConstraint(joint);

            if (m_isMotorSettingsEnabled) {
                JPH::MotorSettings &motor_settings = joint->GetMotorSettings();
                motor_settings.mSpringSettings.mFrequency = m_motorFrequency;
                motor_settings.mSpringSettings.mDamping = m_motorDamping;
                motor_settings.SetForceLimit(m_motorForceLimit);
            }


            return joint;
        }
    };

public:
    PhysicsBuilder();

    static PhysicsBodyBuilder newBody(JPH::BodyInterface &bodyInterface) {
        return PhysicsBodyBuilder(bodyInterface);
    }

    static PhysicsJointBuilder newJoint(JPH::PhysicsSystem &physicsSystem) {
        return PhysicsJointBuilder(physicsSystem);
    }
};