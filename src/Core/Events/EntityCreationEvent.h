#pragma once

#include "Event.h"

#include <utility>

class EntityCreationEvent : public BaseEvent {
    TYPE_DEFINITION(EntityCreationEvent);

public:
    enum Type {
        CREATE,
        CREATE_COMPONENT,
        REMOVE_COMPONENT,
        REMOVE,
        CLONE,
    };

    EntityCreationEvent() : BaseEvent(),
                            m_ConfigurationName(),
                            m_entityId(-1),
                            m_name(),
                            m_Type(CREATE) {
    }

    EntityCreationEvent(Type type, int entityId) : BaseEvent(),
                                                   m_Type(type),
                                                   m_entityId(entityId) {
    }

    EntityCreationEvent(Type type, int entityId, int componentId) : BaseEvent(),
                                                                    m_Type(type),
                                                                    m_entityId(entityId),
                                                                    m_componentId(componentId) {
    }

    EntityCreationEvent(Type type, int entityId, std::string name) : BaseEvent(),
                                                                     m_Type(type),
                                                                     m_entityId(entityId),
                                                                     m_name(std::move(name)) {
    }

    EntityCreationEvent(Type type, std::string name, std::string className) : BaseEvent(),
                                                                              m_Type(type),
                                                                              m_name(std::move(name)),
                                                                              m_ConfigurationName(
                                                                                  std::move(className)),
                                                                              m_entityId(0) {
    }

    Type m_Type;
    std::string m_ConfigurationName;
    std::string m_name;
    int m_entityId;
    int m_componentId{-1};
};
