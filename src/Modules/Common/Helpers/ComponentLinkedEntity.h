#pragma once

#include "../../../Core/API.h"
#include <string>
#include <utility>

class ComponentLinkedEntity {
    inline static const std::string KEY = "linkedEntity";

public:
    ComponentLinkedEntity() : m_isEntityLinked(false),
                              m_hasUnknownLinkedEntityId(false),
                              m_linkedEntityId(0),
                              m_linkedEntityName() {
    }

    void serialize(nlohmann::json &j) {
        if (isLinkedToEntityId()) {
            j[KEY] = m_linkedEntityName;
        }
    }

    void deserialize(nlohmann::json &j) {
        m_linkedEntityName = j.value(KEY, m_linkedEntityName);
        if (!m_linkedEntityName.empty()) {
            m_hasUnknownLinkedEntityId = true;
            m_isEntityLinked = false;
        }
    }

    [[nodiscard]] bool isLinkableToEntity() const {
        return isLinkedToEntityId() || shouldResolveLinkedEntityId();
    }

    [[nodiscard]] bool isLinkedToEntityId() const {
        return m_isEntityLinked;
    }

    [[nodiscard]] bool shouldResolveLinkedEntityId() const {
        return m_hasUnknownLinkedEntityId && !m_linkedEntityName.empty();
    }

    void setLinkedEntityId(const Identity::Type entityId) {
        m_linkedEntityId = entityId;
        m_hasUnknownLinkedEntityId = false;
        m_isEntityLinked = true;
    }

    void setLinkedEntityName(std::string name) {
        m_linkedEntityName = std::move(name);
        m_hasUnknownLinkedEntityId = true;
        m_isEntityLinked = false;
    }

    [[nodiscard]] Identity::Type getLinkedEntityId() const {
        return m_linkedEntityId;
    }

    [[nodiscard]] std::string getLinkedEntityName() const {
        return m_linkedEntityName;
    }

private:
    bool m_isEntityLinked;
    bool m_hasUnknownLinkedEntityId;
    Identity::Type m_linkedEntityId;
    std::string m_linkedEntityName;
};
