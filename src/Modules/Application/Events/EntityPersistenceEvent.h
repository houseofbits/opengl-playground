#pragma once

#include <utility>

#include "../../../Core/API.h"

class EntityPersistenceEvent : public BaseEvent {
    TYPE_DEFINITION(EntityPersistenceEvent);

public:
    enum EventType {
        TYPE_LOAD,
        TYPE_SAVE,
    };

    EntityPersistenceEvent(EventType type, std::string path) : BaseEvent(), m_Type(type), m_path(std::move(path)) {
    }

    EventType m_Type;
    std::string m_path;
};
