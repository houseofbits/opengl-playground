#pragma once

#include "../../../Core/API.h"
#include "../Components/MainCharacterBehaviourComponent.h"

class MainCharacterBehaviourSystem : public EntitySystem {
public:
    MainCharacterBehaviourSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleInputEvent(const InputEvent &);

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

};
