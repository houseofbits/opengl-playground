#pragma once
#include "Event.h"
#include <glm/vec2.hpp>
#include <string>

class InputEvent : public BaseEvent {
    TYPE_DEFINITION(InputEvent);

public:
    enum EventType {
        MOUSEWHEEL,
        MOUSEDOWN,
        MOUSEUP,
        MOUSEMOVE,
        KEYDOWN,
        KEYUP,
        KEYPRESS,
    };

    explicit InputEvent(EventType type) : BaseEvent(),
                                          type(type),
                                          mousePosition(0, 0),
                                          mouseMotion(0, 0),
                                          mouseWheel(0, 0),
                                          keyCode(0),
                                          modKeyShift(false),
                                          modKeyAlt(false),
                                          modKeyCtrl(false),
                                          mouseButtonLeft(false),
                                          mouseButtonRight(false),
                                          textInput() {
    }

    EventType type;
    glm::vec2 mousePosition;
    glm::vec2 mouseMotion;
    glm::vec2 mouseWheel;
    unsigned int keyCode;
    bool modKeyShift;
    bool modKeyAlt;
    bool modKeyCtrl;
    bool mouseButtonLeft;
    bool mouseButtonRight;
    std::string textInput;
};
