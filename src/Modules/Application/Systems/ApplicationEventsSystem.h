#pragma once

#include "../../../Core/API.h"
#include "../Resources/WindowResource.h"
#include "../Events/InputEvent.h"

class ApplicationEventsSystem : public EntitySystem {
public:
    ApplicationEventsSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    bool isRunning;

private:
    void processSDLEvent(EventManager &, SDL_Event &);

    static void onKeyEvent(EventManager &, InputEvent::EventType type, int keysym, bool isAlt, bool isCtrl,
                           bool isdShift);

    static void onMouseEvent(EventManager &, InputEvent::EventType type, glm::vec2 position, glm::vec2 motion,
                             bool mouseLeft, bool mouseRight,
                             bool isAlt, bool isCtrl, bool isShift);

    static void onMouseWheelEvent(EventManager &, glm::vec2 wheel, bool mouseLeft, bool mouseRight, bool isAlt,
                                  bool isCtrl,
                                  bool isShift);

    ResourceHandle<WindowResource> m_WindowResource;
};