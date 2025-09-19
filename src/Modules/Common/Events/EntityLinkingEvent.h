#pragma once

#include <utility>

#include "../../../Core/API.h"

class EntityLinkingEvent : public BaseEvent {
    TYPE_DEFINITION(EntityLinkingEvent);

public:
    enum Type {
        TYPE_LINK_WITH_ID,
        TYPE_LINK_WITH_NAME,
        TYPE_UNLINK,
    };

    EntityLinkingEvent(const Identity::Type componentId, const Identity::Type linkedId) : BaseEvent(),
        m_componentId(componentId),
        m_linkedEntityId(linkedId) {
        m_linkType = TYPE_LINK_WITH_ID;
    }

    EntityLinkingEvent(const Identity::Type componentId, std::string linkedName) : BaseEvent(),
        m_componentId(componentId),
        m_linkedEntityId(0),
        m_linkedEntityName(std::move(linkedName)) {
        m_linkType = TYPE_LINK_WITH_NAME;
    }

    explicit EntityLinkingEvent(const Identity::Type componentId) : BaseEvent(),
                                                                    m_componentId(componentId),
                                                                    m_linkedEntityId(0) {
        m_linkType = TYPE_UNLINK;
    }

    Type m_linkType;
    Identity::Type m_componentId;
    Identity::Type m_linkedEntityId;
    std::string m_linkedEntityName;
};
