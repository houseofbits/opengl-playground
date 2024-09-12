#include "CharacterControllerSystem.h"
#include "../../../Core/Helper/StringUtils.h"
#include "../../../Core/Helper/Time.h"
#include "../../../Core/Helper/Types.h"
#include "../Events/CharacterPickingEvent.h"

using namespace physx;

static physx::PxControllerFilters filters;

CharacterControllerSystem::CharacterControllerSystem() : EntitySystem(),
                                                         m_isSimulationDisabled(false),
                                                         m_movementDirection(),
                                                         m_isOnGround(false),
                                                         m_doMove(false),
                                                         m_doJump(false),
                                                         m_doInteract(false),
                                                         m_jumpPower(0),
                                                         m_PhysicsResource() {
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<CharacterControllerComponent>();
}

void CharacterControllerSystem::initialize(ResourceManager *resourceManager) {
    resourceManager->request(m_PhysicsResource, "physics");
}

void CharacterControllerSystem::process() {
    if (!m_PhysicsResource.isReady()) {
        return;
    }

    updateCCTs();
}

void CharacterControllerSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &CharacterControllerSystem::handleEditorUIEvent);
    eventManager->registerEventReceiver(this, &CharacterControllerSystem::handleInputEvent);
}

bool CharacterControllerSystem::handleEditorUIEvent(EditorUIEvent *event) {
    if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_ENABLED) {
        m_isSimulationDisabled = false;
    } else if (event->m_Type == EditorUIEvent::TOGGLE_SIMULATION_DISABLED) {
        m_isSimulationDisabled = true;
    } else if (event->m_Type == EditorUIEvent::RESET_TO_INITIAL_TRANSFORM) {
        resetToInitialTransform();
        m_isSimulationDisabled = true;
    }

    return true;
}

bool CharacterControllerSystem::handleInputEvent(InputEvent *event) {
    for (const auto comp: getComponentContainer<CharacterControllerComponent>()) {
        auto *cameraComp = getComponent<CameraComponent>(comp.first);
//        if (cameraComp && cameraComp->m_isActive) {
            processCCTInput(cameraComp, comp.second, event);
//        }
    }

    return true;
}

void CharacterControllerSystem::resetToInitialTransform() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        if (component.second->m_pxRigidActor != nullptr) {
            auto *transform = getComponent<TransformComponent>(component.first);
            transform->m_transform = transform->m_initialTransform;
        }
    }
}

void CharacterControllerSystem::updateCCTs() {
    for (const auto component: getComponentContainer<CharacterControllerComponent>()) {
        auto *transform = getComponent<TransformComponent>(component.first);
        glm::vec3 position = transform->getTranslation();

        if (component.second->m_pxRigidActor == nullptr) {
            component.second->create(*transform);
            m_PhysicsResource().m_pxScene->addActor(*component.second->m_pxRigidActor);
        } else {
            component.second->update(*transform, !m_isSimulationDisabled);

            auto *cameraComp = getComponent<CameraComponent>(component.first);
            if (cameraComp == nullptr) {
                return;
            }

            if (!m_isSimulationDisabled) {
                float capsuleRadius = component.second->m_radius;
                float adjustStep = 0;
                if (!m_PhysicsResource().m_entityContacts[component.first].empty()) {
                    float stepContactHeight = 0;
                    for (const auto point: m_PhysicsResource().m_entityContacts[component.first]) {
                        auto pos = point - position;

                        if (pos.y < 0.01) {
                            m_isOnGround = true;
                            continue;
                        }

                        //Contact above step height
                        if (!m_doMove || pos.y > capsuleRadius) {
                            stepContactHeight = 0;
                            //!!!handle walls while climbing steps
                            //if the dot between pos and movement dir < 0.1 (e.g. perpendicular) allow moving on stairs but ignore this contact
                            break;
                        }

                        auto dir = glm::normalize(pos);
                        float heading = glm::dot(dir, m_movementDirection);
                        if (heading > 0 && pos.y > stepContactHeight) {
                            stepContactHeight = pos.y;
                        }
                    }

                    if (stepContactHeight > 0) {
                        adjustStep = stepContactHeight;
                        std::cout<<stepContactHeight<<std::endl;
                    }
                } else {
                    m_isOnGround = false;
                }

                auto *actor = component.second->m_pxRigidActor;
                if (m_doMove || m_doJump || !m_isOnGround) {
                    PxVec3 force(0);
                    if (m_doMove) {
                        float f = 150;//m_isOnGround ? 150 : 50;
                        force = Types::GLMtoPxVec3(glm::normalize(m_movementDirection) * f);
                    }

                    float yF = -50;
                    if (adjustStep > 0 && !m_doJump) {
                        yF = adjustStep * 1000.0f;
                    }

                    if (m_isOnGround && m_doJump) {
                        yF = 500;
                    }

                    actor->setForceAndTorque(PxVec3(force.x, yF, force.z), PxVec3(0.0f));

                    m_movementDirection = glm::vec3(0);
                    m_doMove = false;
                    m_doJump = false;
                }
            }


            if (cameraComp->m_isActive) {
                cameraComp->m_Camera.setPosition(transform->getTranslation() + glm::vec3(0, component.second->m_height, 0));

                RayCastResult hit;
                if (m_PhysicsResource().characterRayCast(cameraComp->m_Camera.position, cameraComp->m_Camera.getViewDirection(), component.first, hit)) {
                    auto *e = new CharacterPickingEvent();
                    e->m_entityId = hit.m_entityId;
                    e->m_distance = hit.m_distance;
                    e->m_touchPoint = hit.m_touchPoint;
                    e->m_doActivate = m_doInteract;
                    m_EventManager->queueEvent(e);
                }
            }
        }
    }

    m_doInteract = false;
}

void CharacterControllerSystem::processCCTInput(CameraComponent *camera, CharacterControllerComponent *cct, InputEvent *event) {
    glm::vec3 viewDirection = camera->m_Camera.getViewDirection();
    viewDirection.y = 0;
    m_direction = glm::normalize(viewDirection);

    glm::vec3 rightDirection = glm::normalize(glm::cross(m_direction, glm::vec3(0, 1, 0)));

    //W
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 26) {
        m_doMove = true;
        m_movementDirection = m_movementDirection + m_direction;
    }
    //S
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 22) {
        m_doMove = true;
        m_movementDirection = m_movementDirection - m_direction;
    }
    //A
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 4) {
        m_doMove = true;
        m_movementDirection = m_movementDirection - rightDirection;
    }
    //D
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 7) {
        m_doMove = true;
        m_movementDirection = m_movementDirection + rightDirection;
    }
    //Space-bar
    if (event->type == InputEvent::KEYPRESS && event->keyCode == 44) {
        if (m_isOnGround) {
            m_doJump = true;
            m_jumpPower = 30;
        }
    }

    glm::vec3 position = camera->m_Camera.getPosition();
    auto tbn = camera->calculateTBN(camera->m_Camera.getViewDirection());
    if (camera->m_isActive && event->type == InputEvent::MOUSEMOVE && event->mouseButtonLeft) {
        float moveSpeed = 4;
        float lookSpeed = 0.15;

        glm::vec3 rightHorizontal = glm::normalize(glm::cross(tbn.view, glm::vec3(0, 1, 0)));
        glm::vec3 upVertical = glm::normalize(glm::cross(tbn.view, rightHorizontal));

        float rate = lookSpeed * Time::frameTime * -event->mouseMotion.x;
        glm::vec3 viewChange = rightHorizontal * rate;
        tbn = camera->calculateTBN(tbn.view + viewChange);

        rate = lookSpeed * Time::frameTime * -event->mouseMotion.y;
        viewChange = upVertical * rate;
        tbn = camera->calculateTBN(tbn.view + viewChange);

        camera->m_Camera.setView(tbn.view, tbn.up);
    }

    if (event->type == InputEvent::MOUSEDOWN && event->mouseButtonRight) {
        m_doInteract = true;
    }

    if (m_doMove) {
        m_movementDirection = glm::normalize(m_movementDirection);
    }
}