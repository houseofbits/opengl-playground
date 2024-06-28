#pragma once

#include "Event.h"

class EntityCreationEvent : public BaseEvent
{
    TYPE_DEFINITION(EntityCreationEvent);

public:
    std::string m_Name;
};