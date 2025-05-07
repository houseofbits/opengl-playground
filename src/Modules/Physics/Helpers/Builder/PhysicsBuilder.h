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
#include <Jolt/Physics/Constraints/SwingTwistConstraint.h>
#include <Jolt/Physics/Constraints/HingeConstraint.h>
#include <Jolt/Physics/Constraints/DistanceConstraint.h>
#include <Jolt/Physics/Constraints/SliderConstraint.h>
#include "../../../../Core/Reflection/Identity.h"
#include "../Layers.h"
#include "../PhysicsTypeCast.h"
#include "../PhysicsBodyUserData.h"
#include "../../../../Core/Helper/Math.h"
#include "../../../Common/Components/TransformComponent.h"
#include "../../Components/PhysicsBodyComponent.h"
#include "../CompoundShape/PhysicsCompoundShapeBuilder.h"
#include "Jolt/Physics/Collision/Shape/StaticCompoundShape.h"

class PhysicsBuilder {
    class PhysicsBodyBuilder {
        JPH::PhysicsSystem *m_physicsSystem = nullptr;
        Identity::Type m_entityId = 0;
        JPH::PhysicsMaterial *m_material = nullptr;
        JPH::BodyCreationSettings m_settings;
        bool m_isSensor = false;
        bool m_excludeSensorFromActionHit = false;

        [[nodiscard]] JPH::Body *createInner() const {
            const auto physicsBody = m_physicsSystem->GetBodyInterface().CreateBody(m_settings);

            if (m_entityId != 0) {
                auto *userData = new PhysicsBodyUserData(m_entityId);
                physicsBody->SetUserData(reinterpret_cast<unsigned long>(userData));
            }

            m_physicsSystem->GetBodyInterface().AddBody(physicsBody->GetID(), JPH::EActivation::Activate);

            return physicsBody;
        }

    public:
        explicit PhysicsBodyBuilder(JPH::PhysicsSystem &physicsSystem): m_physicsSystem(&physicsSystem), m_settings() {
        }

        PhysicsBodyBuilder &setEntityReference(const Identity::Type entityId) {
            m_entityId = entityId;

            return *this;
        }

        PhysicsBodyBuilder &setSensor(const bool isSensor, const bool disableActions = false) {
            m_isSensor = isSensor;
            m_excludeSensorFromActionHit = disableActions;

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

        PhysicsBodyBuilder &setShape(const JPH::Shape &shape) {
            m_settings.SetShape(&shape);

            return *this;
        }

        PhysicsBodyBuilder &addStaticCompoundShape(const JPH::StaticCompoundShapeSettings &shapeSettings) {
            m_settings.SetShapeSettings(&shapeSettings);

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
            if (m_isSensor) {
                m_settings.mObjectLayer = m_excludeSensorFromActionHit ? Layers::SENSOR : Layers::SENSOR_WITH_ACTIONS;
                m_settings.mIsSensor = true;
            }

            return createInner();
        }

        JPH::Body *createStatic() {
            m_settings.mMotionType = JPH::EMotionType::Static;
            m_settings.mObjectLayer = Layers::NON_MOVING;
            if (m_isSensor) {
                m_settings.mObjectLayer = m_excludeSensorFromActionHit ? Layers::SENSOR : Layers::SENSOR_WITH_ACTIONS;
                m_settings.mIsSensor = true;
            }

            return createInner();
        }
    };

    class PhysicsJointBuilder {
        JPH::PhysicsSystem *m_physicsSystem = nullptr;
        JPH::BodyID m_bodyId1{};
        JPH::BodyID m_bodyId2{};
        glm::vec2 m_limits;
        bool m_isLimitsEnabled = false;
        bool m_isMotorSettingsEnabled = false;
        float m_motorFrequency = 0;
        float m_motorDamping = 0;
        float m_motorForceLimit = FLT_MAX;
        glm::mat4 m_attachment1{1.0};
        glm::mat4 m_attachment2{1.0};
        struct {
            glm::vec2 m_twistAngleLimits{0, 0};
            glm::vec2 m_coneHalfAngle{0, 0};
        } m_swingTwistSettings{};

    public:
        explicit PhysicsJointBuilder(JPH::PhysicsSystem &physicsSystem): m_physicsSystem(&physicsSystem), m_limits() {
        }

        PhysicsJointBuilder &setBodies(const JPH::BodyID &body1, const JPH::BodyID &body2) {
            m_bodyId1 = body1;
            m_bodyId2 = body2;

            return *this;
        }

        PhysicsJointBuilder &setBodies(const PhysicsBodyComponent &body1, const PhysicsBodyComponent &body2) {
            m_bodyId1 = body1.m_physicsBodyId;
            m_bodyId2 = body2.m_physicsBodyId;

            return *this;
        }

        PhysicsJointBuilder &setLimits(const glm::vec2 limits) {
            m_limits = limits;
            m_isLimitsEnabled = true;

            return *this;
        }

        PhysicsJointBuilder &setAttachments(const glm::mat4 &attachment1, const glm::mat4 &attachment2) {
            m_attachment1 = attachment1;
            m_attachment2 = attachment2;

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

        PhysicsJointBuilder &setTwistAngleLimits(const glm::vec2 limit) {
            m_swingTwistSettings.m_twistAngleLimits = limit;

            return *this;
        }

        PhysicsJointBuilder &setConeHalfAngle(const glm::vec2 coneHalfAngle) {
            m_swingTwistSettings.m_coneHalfAngle = coneHalfAngle;

            return *this;
        }

        [[nodiscard]] JPH::Ref<JPH::FixedConstraint> createFixedConstraint() const {
            if (m_bodyId1.IsInvalid() || m_bodyId2.IsInvalid()) {
                return nullptr;
            }

            JPH::Body *body1 = nullptr;
            JPH::Body *body2 = nullptr;

            if (JPH::BodyLockWrite lock1(m_physicsSystem->GetBodyLockInterface(), m_bodyId1); lock1.Succeeded()) {
                body1 = &lock1.GetBody();
                lock1.ReleaseLock();
            }

            if (JPH::BodyLockWrite lock2(m_physicsSystem->GetBodyLockInterface(), m_bodyId2); lock2.Succeeded()) {
                body2 = &lock2.GetBody();
                lock2.ReleaseLock();
            }

            // Log::write("Hinge joint params: ", body1, " ,", body2);

            if (!body1 || !body2) {
                return nullptr;
            }

            JPH::FixedConstraintSettings settings;
            settings.mAutoDetectPoint = true;
            settings.mPoint1 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment1));
            settings.mPoint2 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment2));
            const auto joint = settings.Create(*body1, *body2);

            m_physicsSystem->AddConstraint(joint);

            return dynamic_cast<JPH::FixedConstraint *>(joint);
        }

        [[nodiscard]] JPH::Ref<JPH::DistanceConstraint> createDistanceConstraint() const {
            if (m_bodyId1.IsInvalid() || m_bodyId2.IsInvalid()) {
                return nullptr;
            }

            JPH::Body *body1 = nullptr;
            JPH::Body *body2 = nullptr;

            JPH::BodyLockWrite lock1(m_physicsSystem->GetBodyLockInterface(), m_bodyId1);
            if (lock1.Succeeded()) {
                body1 = &lock1.GetBody();
                lock1.ReleaseLock();
            }

            JPH::BodyLockWrite lock2(m_physicsSystem->GetBodyLockInterface(), m_bodyId2);
            if (lock2.Succeeded()) {
                body2 = &lock2.GetBody();
                lock2.ReleaseLock();
            }

            if (!body1 || !body2) {
                return nullptr;
            }

            JPH::DistanceConstraintSettings settings;
            settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM;
            settings.mPoint1 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment1));
            settings.mPoint2 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment2));
            settings.mMinDistance = m_limits.x;
            settings.mMaxDistance = m_limits.y;

            auto joint = dynamic_cast<JPH::DistanceConstraint *>(settings.Create(*body1, *body2));

            m_physicsSystem->AddConstraint(joint);

            return joint;
        }

        [[nodiscard]] JPH::Ref<JPH::SliderConstraint> createSliderConstraint() const {
            if (m_bodyId1.IsInvalid() || m_bodyId2.IsInvalid()) {
                return nullptr;
            }

            JPH::Body *body1 = nullptr;
            JPH::Body *body2 = nullptr;

            JPH::BodyLockWrite lock1(m_physicsSystem->GetBodyLockInterface(), m_bodyId1);
            if (lock1.Succeeded()) {
                body1 = &lock1.GetBody();
                lock1.ReleaseLock();
            }

            JPH::BodyLockWrite lock2(m_physicsSystem->GetBodyLockInterface(), m_bodyId2);
            if (lock2.Succeeded()) {
                body2 = &lock2.GetBody();
                lock2.ReleaseLock();
            }

            if (!body1 || !body2) {
                return nullptr;
            }

            JPH::SliderConstraintSettings settings;
            settings.mAutoDetectPoint = true;
            settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM;
            settings.mPoint1 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment1));
            settings.mPoint2 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment2));
            settings.mSliderAxis1 = PhysicsTypeCast::glmToJPH(Math::getXAxis(m_attachment1));
            settings.mSliderAxis2 = PhysicsTypeCast::glmToJPH(Math::getXAxis(m_attachment2));
            settings.mNormalAxis1 = PhysicsTypeCast::glmToJPH(Math::getYAxis(m_attachment1));
            settings.mNormalAxis2 = PhysicsTypeCast::glmToJPH(Math::getYAxis(m_attachment2));

            if (m_isLimitsEnabled) {
                settings.mLimitsMin = m_limits.x;
                settings.mLimitsMax = m_limits.y;
            }

            auto joint = dynamic_cast<JPH::SliderConstraint *>(settings.Create(*body1, *body2));

            m_physicsSystem->AddConstraint(joint);

            if (m_isMotorSettingsEnabled) {
                JPH::MotorSettings &motor_settings = joint->GetMotorSettings();
                motor_settings.mSpringSettings.mFrequency = m_motorFrequency;
                motor_settings.mSpringSettings.mDamping = m_motorDamping;
                motor_settings.SetForceLimit(m_motorForceLimit);
            }

            return joint;
        }

        [[nodiscard]] JPH::Ref<JPH::HingeConstraint> createHingeConstraint() const {
            if (m_bodyId1.IsInvalid() || m_bodyId2.IsInvalid()) {
                return nullptr;
            }

            JPH::HingeConstraintSettings settings;
            settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM;
            settings.mPoint1 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment1));
            settings.mPoint2 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment2));
            settings.mHingeAxis1 = PhysicsTypeCast::glmToJPH(Math::getXAxis(m_attachment1));
            settings.mHingeAxis2 = PhysicsTypeCast::glmToJPH(Math::getXAxis(m_attachment2));
            settings.mNormalAxis1 = PhysicsTypeCast::glmToJPH(Math::getYAxis(m_attachment1));
            settings.mNormalAxis2 = PhysicsTypeCast::glmToJPH(Math::getYAxis(m_attachment2));

            if (m_isLimitsEnabled) {
                settings.mLimitsMin = m_limits.x;
                settings.mLimitsMax = m_limits.y;
            }

            JPH::Body *body1 = nullptr;
            JPH::Body *body2 = nullptr;

            body1 = m_physicsSystem->GetBodyLockInterface().TryGetBody(m_bodyId1);
            body2 = m_physicsSystem->GetBodyLockInterface().TryGetBody(m_bodyId2);

            // JPH::BodyLockWrite lock1(m_physicsSystem->GetBodyLockInterface(), m_bodyId1);
            // if (lock1.Succeeded()) {
            //     body1 = &lock1.GetBody();
            //     lock1.ReleaseLock();
            // }
            //
            // JPH::BodyLockWrite lock2(m_physicsSystem->GetBodyLockInterface(), m_bodyId2);
            // if (lock2.Succeeded()) {
            //     body2 = &lock2.GetBody();
            //     lock2.ReleaseLock();
            // }

            if (!body1 || !body2) {
                return nullptr;
            }

            const auto joint = dynamic_cast<JPH::HingeConstraint *>(settings.Create(*body1, *body2));

            m_physicsSystem->AddConstraint(joint);

            if (m_isMotorSettingsEnabled) {
                JPH::MotorSettings &motor_settings = joint->GetMotorSettings();
                motor_settings.mSpringSettings.mFrequency = m_motorFrequency;
                motor_settings.mSpringSettings.mDamping = m_motorDamping;
                motor_settings.SetForceLimit(m_motorForceLimit);
            }

            // Log::write("Create hinge joint: ", body1, " ", body2);

            return joint;
        }

        [[nodiscard]] JPH::Ref<JPH::SwingTwistConstraint> createSwingTwistConstraint() const {
            JPH::Body *body1 = nullptr;
            JPH::Body *body2 = nullptr;

            body1 = m_physicsSystem->GetBodyLockInterface().TryGetBody(m_bodyId1);
            body2 = m_physicsSystem->GetBodyLockInterface().TryGetBody(m_bodyId2);

            if (!body1 || !body2) {
                return nullptr;
            }

            JPH::SwingTwistConstraintSettings settings;
            settings.mSpace = JPH::EConstraintSpace::LocalToBodyCOM; //JPH::EConstraintSpace::WorldSpace;
            settings.mPosition1 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment1));
            settings.mPosition2 = PhysicsTypeCast::glmToJPH(Math::getTranslation(m_attachment2));
            settings.mTwistAxis1 = PhysicsTypeCast::glmToJPH(Math::getXAxis(m_attachment1));
            settings.mTwistAxis2 = PhysicsTypeCast::glmToJPH(Math::getXAxis(m_attachment2));
            settings.mPlaneAxis1 = PhysicsTypeCast::glmToJPH(Math::getYAxis(m_attachment1));
            settings.mPlaneAxis2 = PhysicsTypeCast::glmToJPH(Math::getYAxis(m_attachment2));
            settings.mTwistMinAngle = m_swingTwistSettings.m_twistAngleLimits.x;
            settings.mTwistMaxAngle = m_swingTwistSettings.m_twistAngleLimits.y;
            settings.mNormalHalfConeAngle = m_swingTwistSettings.m_coneHalfAngle.x;
            settings.mPlaneHalfConeAngle = m_swingTwistSettings.m_coneHalfAngle.y;

            const auto joint = dynamic_cast<JPH::SwingTwistConstraint *>(settings.Create(*body1, *body2));

            m_physicsSystem->AddConstraint(joint);

            return joint;
        }
    };

public:
    PhysicsBuilder();

    static PhysicsBodyBuilder newBody(JPH::PhysicsSystem &physicsSystem) {
        return PhysicsBodyBuilder(physicsSystem);
    }

    static PhysicsJointBuilder newJoint(JPH::PhysicsSystem &physicsSystem) {
        return PhysicsJointBuilder(physicsSystem);
    }
};
