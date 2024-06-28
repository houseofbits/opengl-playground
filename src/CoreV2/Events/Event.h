#pragma once
#include "../../TypedClass.h"

class EventManager;

class BaseEvent : public BaseClass
{
public:
    EventManager* m_EventManager{nullptr};

    void queue();
    void trigger();
};
