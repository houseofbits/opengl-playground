#pragma once
#include "Event.h"
#include <glm/vec2.hpp>
#include <string>
#include <utility>

class InputEvent : public BaseEvent {
    TYPE_DEFINITION(InputEvent);

public:
    enum EventType {
        MOUSEDOWN,
        MOUSEUP,
        MOUSEENTER,
        MOUSELEAVE,
        MOUSEMOVE,
        KEYDOWN,
        KEYUP,
        KEYPRESS,
        DRAG,
        TEXTENTER,
    };

    explicit InputEvent(EventType type) : BaseEvent(),
                                          type(type),
                                          mousePosition(0, 0),
                                          mouseMotion(0, 0),
                                          keyCode(0),
                                          modKeyShift(false),
                                          modKeyAlt(false),
                                          modKeyCtrl(false),
                                          mouseButtonLeft(false),
                                          mouseButtonRight(false),
                                          textInput() {}

    EventType type;
    glm::vec2 mousePosition;
    glm::vec2 mouseMotion;
    unsigned int keyCode;
    bool modKeyShift;
    bool modKeyAlt;
    bool modKeyCtrl;
    bool mouseButtonLeft;
    bool mouseButtonRight;
    std::string textInput;
};