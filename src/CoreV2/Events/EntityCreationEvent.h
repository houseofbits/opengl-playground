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
                            m_Name(),
                            m_entityId(-1),
                            m_Type(CREATE) {
    }

    Type m_Type;
    std::string m_Name;
    int m_entityId;
};