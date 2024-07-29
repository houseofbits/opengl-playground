#include "Event.h"
#include "EventManager.h"

void BaseEvent::queue() {
    if (m_EventManager != nullptr) {
        m_EventManager->queueEvent(this);
    }
}

void BaseEvent::trigger() {
    if (m_EventManager != nullptr) {
        m_EventManager->triggerEvent(this);
    }
}
