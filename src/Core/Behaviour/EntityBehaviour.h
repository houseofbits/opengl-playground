#pragma once

#include "../Json/Json.h"
#include "../Resources/ResourceManager.h"
#include "../Reflection/TypedClass.h"
#include "../Events/EventManager.h"

class Entity;

class EntityBehaviour : public BaseClass, public EventHandler {
public:
    typedef std::shared_ptr<EntityBehaviour> TEntityBehaviourPtr;

    EntityBehaviour();

    virtual ~EntityBehaviour() = default;

    virtual void serialize(nlohmann::json &j) {};

    virtual void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {};

    virtual void initialize(ResourceManager &) {};

    virtual void registerEventHandlers(EventManager &eventManager) = 0;

    virtual std::string getDisplayName() {
        return "";
    }

    virtual std::string getDescription() {
        return "";
    }

    Identity::Type getId() override;

    void setEntity(Entity *e) {
        m_Entity = e;
    }

    void setEventManager(EventManager *ev) {
        m_EventManager = ev;
    }

    void detachFromEntity();

protected:
    Entity *m_Entity;
    EventManager *m_EventManager;
};
