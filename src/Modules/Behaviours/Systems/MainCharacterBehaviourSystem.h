#pragma once

#include "../../../Core/API.h"
#include "../Components/MainCharacterBehaviourComponent.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../../Common/Components/CameraComponent.h"

class MainCharacterBehaviourSystem : public EntitySystem {
public:
    MainCharacterBehaviourSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleInputEvent(const InputEvent &);

    void handleSystemEvent(const SystemEvent &);

private:
    void handleMovement(const InputEvent &, TransformComponent *,
                               PhysicsCharacterComponent *);

    void handleAction(const InputEvent &, PhysicsCharacterComponent *, glm::vec3 viewPosition,
                             glm::vec3 viewDirection) const;

    void handleMouseLook(const InputEvent &, MainCharacterBehaviourComponent *, PhysicsCharacterComponent *);

    void updateCamera(TransformComponent *,const MainCharacterBehaviourComponent *, glm::vec3 viewPosition) const;

    EntityRelatedComponentRegistry<TransformComponent, PhysicsCharacterComponent, MainCharacterBehaviourComponent> *
    m_registry;

    bool m_isEditorModeEnabled;
};
