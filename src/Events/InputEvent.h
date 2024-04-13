#pragma once
#include "Event.h"
#include <glm/vec2.hpp>
#include <string>

class InputEvent : public BaseEvent
{
    TYPE_DEFINITION(InputEvent);

public:
    enum EventType
    {
        MOUSEDOWN,
        MOUSEUP,
        MOUSEENTER,
        MOUSELEAVE,
        MOUSEMOVE,
        KEYDOWN,
        KEYUP,
        KEYPRESS,
        DRAG,
        TEXENTER,
    };

    InputEvent() : type(MOUSEDOWN),
                   mousePosition(),
                   mouseMotion(),
                   keyCode(0),
                   mouseButtonLeft(false),
                   mouseButtonRight(false),
                   textInput() {}
    InputEvent(EventType type, unsigned int key) : type(type), keyCode(key) {}
    InputEvent(EventType type, glm::vec2 mousePosition, bool mouseLeft, bool mouseRight) : type(type),
                                                                                           mousePosition(mousePosition),
                                                                                           mouseButtonLeft(mouseLeft),
                                                                                           mouseButtonRight(mouseRight) {}
    InputEvent(EventType type, glm::vec2 mousePosition, glm::vec2 mouseRel) : type(type),
                                                                              mousePosition(mousePosition),
                                                                              mouseMotion(mouseRel) {}
    InputEvent(EventType type, glm::vec2 mousePosition, glm::vec2 mouseRel, bool mouseLeft, bool mouseRight) : type(type),
                                                                                                               mousePosition(mousePosition),
                                                                                                               mouseMotion(mouseRel),
                                                                                                               mouseButtonLeft(mouseLeft),
                                                                                                               mouseButtonRight(mouseRight) {}
    InputEvent(EventType type, std::string text) : type(type), textInput(text) {}
    InputEvent(EventType type) : type(type) {}

    EventType type;
    glm::vec2 mousePosition;
    glm::vec2 mouseMotion;
    unsigned int keyCode;
    bool mouseButtonLeft;
    bool mouseButtonRight;
    std::string textInput;
    bool modKeyShift;
    bool modKeyAlt;
    bool modKeyCtrl;
};