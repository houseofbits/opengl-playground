#pragma once

class Identity {
public:
    typedef unsigned int Type;
    inline static unsigned int m_LastId = 0;
    enum Subject {
        UNDEFINED,
        ENTITY,
        COMPONENT,
    };

    Identity() {
        m_id = 0;
        m_Subject = UNDEFINED;
    }

    Identity(Identity &id) {
        m_id = id.m_id;
        m_Subject = id.m_Subject;
    }

    Identity(const Identity &id) {
        m_id = id.m_id;
        m_Subject = id.m_Subject;
    }

    static Identity fromEntityId(Type id) {
        Identity i;
        i.m_id = id;
        i.m_Subject = ENTITY;

        return i;
    }

    static Identity fromComponentId(Type id) {
        Identity i;
        i.m_id = id;
        i.m_Subject = COMPONENT;

        return i;
    }

    static Identity create(Subject subject) {
        m_LastId++;
        return Identity(m_LastId, subject);
    }

    bool operator==(const Identity &other) const {
        return m_id == other.m_id && m_Subject == other.m_Subject;
    }

    Type operator()() const {
        return m_id;
    }

    [[nodiscard]] Type id() const {
        return m_id;
    }

    bool isUndefined() {
        return m_Subject == UNDEFINED || m_id == 0;
    }

private:
    explicit Identity(Type id, Subject subject) {
        m_id = id;
        m_Subject = subject;
    }

    Type m_id;
    Subject m_Subject;
};