#pragma once

#include "../../../Core/API.h"
#include "../Components/MainCharacterBehaviourComponent.h"
#include "../../Physics/Components/PhysicsCharacterComponent.h"
#include "../../Common/Components/CameraComponent.h"

class MainCharacterBehaviourSystem : public EntitySystem {
public:
    MainCharacterBehaviourSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleInputEvent(const InputEvent &);

private:

    static void handleMouseLook(const InputEvent &, PhysicsCharacterComponent *, CameraComponent *);

    static void handleMovement(const InputEvent &, PhysicsCharacterComponent *, CameraComponent *);

    static void handleAction(const InputEvent &, PhysicsCharacterComponent *, CameraComponent *);

    Entity *getCharacterEntity() {
        auto c = findComponent<MainCharacterBehaviourComponent>(
                [](MainCharacterBehaviourComponent *c) {
                    return c->m_isActive;
                });
        if (!c) {
            return nullptr;
        }

        return m_EntityContext->getEntity(c->m_EntityId.id());
    }

    RelatedComponentRegistry<CameraComponent, PhysicsCharacterComponent, MainCharacterBehaviourComponent>* m_registry;
};
