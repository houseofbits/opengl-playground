#pragma once

#include "Event.h"

class EntityCreationEvent : public BaseEvent {
    TYPE_DEFINITION(EntityCreationEvent);

public:
    enum Type {
        CREATE,
        REMOVE
    };

    EntityCreationEvent() : BaseEvent(),
                            m_ConfigurationName(),
                            m_entityId(-1),
                            m_name(),
                            m_Type(CREATE) {
    }

    Type m_Type;
    std::string m_ConfigurationName;
    std::string m_name;
    int m_entityId;
};