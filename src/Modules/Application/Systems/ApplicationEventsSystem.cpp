#include "ApplicationEventsSystem.h"
#include "../Events/RawSDLEvent.h"
#include "../../../Core/Helper/Profiling.h"

ApplicationEventsSystem::ApplicationEventsSystem() : EntitySystem(), isRunning(true),
                                                     m_WindowResource() {
}

void ApplicationEventsSystem::initialize(ResourceManager &resourceManager, EventManager &) {
    resourceManager.request(m_WindowResource, "window");
}

void ApplicationEventsSystem::process(EventManager &eventManager) {
    // if (!m_WindowResource().pollEvents()) {
    //     isRunning = false;
    // }

    if (m_WindowResource().m_Window == nullptr) {
        return;
    }

    SDL_Event sdl_event;

    processSDLEvent(eventManager, sdl_event);
}

void ApplicationEventsSystem::registerEventHandlers(EventManager &eventManager) {
}

void ApplicationEventsSystem::processSDLEvent(EventManager &eventManager, SDL_Event &sdl_event2) {
    SDL_Event sdl_event;
    SDL_Keymod modKeys = SDL_GetModState();
    bool isShift = modKeys & KMOD_SHIFT;
    bool isCtrl = modKeys & KMOD_CTRL;
    bool isAlt = modKeys & KMOD_ALT;
    bool leftButton = false;
    bool rightButton = false;

    while (SDL_PollEvent(&sdl_event) != 0) {
        eventManager.queueEvent<RawSDLEvent>(sdl_event);

        if (sdl_event.type == SDL_QUIT) {
            isRunning = false;

            return;
        } else if (sdl_event.type == SDL_KEYDOWN) {
            switch (sdl_event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    isRunning = false;

                    return;
                case SDLK_F12:
                    m_EntityContext->entitySystemRegistry.printProfilingData();
                    break;
                // case SDLK_F11:
                //     isFullScreen = !isFullScreen;
                //
                //     if (isFullScreen) {
                //         SDL_SetWindowFullscreen(sdlWindow, windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                //         SDL_GetWindowSize(sdlWindow, &viewportWidth, &viewportHeight);
                //     } else {
                //         viewportWidth = 1920;
                //         viewportHeight = 1080;
                //         SDL_SetWindowFullscreen(sdlWindow, windowFlags);
                //     }
                //
                //     eventManager->queueEvent<SystemEvent>(SystemEvent::Type::WINDOW_RESIZED, this);
                //     break;
                default: ;
            }
        }

        if (sdl_event.button.button == SDL_BUTTON_LEFT) {
            leftButton = true;
        }
        if (sdl_event.button.button == SDL_BUTTON_RIGHT || sdl_event.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            rightButton = true;
        }

        switch (sdl_event.type) {
            case SDL_KEYDOWN:
                onKeyEvent(eventManager, InputEvent::KEYDOWN, sdl_event.key.keysym.scancode, isAlt, isCtrl, isShift);
                break;
            case SDL_KEYUP:
                onKeyEvent(eventManager, InputEvent::KEYUP, sdl_event.key.keysym.scancode, isAlt, isCtrl, isShift);
                break;
            case SDL_MOUSEWHEEL:
                onMouseWheelEvent(
                    eventManager,
                    glm::vec2(static_cast<float>(sdl_event.wheel.x), static_cast<float>(sdl_event.wheel.y)),
                    leftButton,
                    rightButton,
                    isAlt,
                    isCtrl,
                    isShift);
                break;
            case SDL_MOUSEMOTION:
                onMouseEvent(
                    eventManager,
                    InputEvent::MOUSEMOVE,
                    glm::vec2(static_cast<float>(sdl_event.motion.x), static_cast<float>(sdl_event.motion.y)),
                    glm::vec2(static_cast<float>(sdl_event.motion.xrel), static_cast<float>(sdl_event.motion.yrel)),
                    leftButton,
                    rightButton,
                    isAlt,
                    isCtrl,
                    isShift);
                break;
            case SDL_MOUSEBUTTONDOWN:
                onMouseEvent(
                    eventManager,
                    InputEvent::MOUSEDOWN,
                    glm::vec2(static_cast<float>(sdl_event.motion.x), static_cast<float>(sdl_event.motion.y)),
                    glm::vec2(static_cast<float>(sdl_event.motion.xrel), static_cast<float>(sdl_event.motion.yrel)),
                    leftButton,
                    rightButton,
                    isAlt,
                    isCtrl,
                    isShift);
                break;
            case SDL_MOUSEBUTTONUP:
                onMouseEvent(
                    eventManager,
                    InputEvent::MOUSEUP,
                    glm::vec2(static_cast<float>(sdl_event.motion.x), static_cast<float>(sdl_event.motion.y)),
                    glm::vec2(static_cast<float>(sdl_event.motion.xrel), static_cast<float>(sdl_event.motion.yrel)),
                    leftButton,
                    rightButton,
                    isAlt,
                    isCtrl,
                    isShift);
                break;

            case SDL_TEXTINPUT:
                //            eventManager->queueEvent(new InputEvent(InputEvent::TEXTENTER, std::string(sdl_event.text.text)));
                break;
            default:
                ;
        };
    }

    int numKeys;
    const Uint8 *state = SDL_GetKeyboardState(&numKeys);
    if (numKeys > 0) {
        for (int i = 0; i < numKeys; i++) {
            if (state[i] > 0) {
                onKeyEvent(eventManager, InputEvent::KEYPRESS, i, isAlt, isCtrl, isShift);
            }
        }
    }
}

void ApplicationEventsSystem::onKeyEvent(EventManager &eventManager, InputEvent::EventType type, int keysym, bool isAlt,
                                         bool isCtrl, bool isShift) {
    auto &event = eventManager.createEvent<InputEvent>(type);
    event.keyCode = keysym;
    event.modKeyAlt = isAlt;
    event.modKeyShift = isShift;
    event.modKeyCtrl = isCtrl;
    eventManager.queueEvent(event);
}

void ApplicationEventsSystem::onMouseEvent(EventManager &eventManager, InputEvent::EventType type, glm::vec2 position,
                                           glm::vec2 motion, bool mouseLeft,
                                           bool mouseRight, bool isAlt, bool isCtrl, bool isShift) {
    auto &event = eventManager.createEvent<InputEvent>(type);
    event.mousePosition = position;
    event.mouseMotion = motion;
    event.mouseButtonLeft = mouseLeft;
    event.mouseButtonRight = mouseRight;
    event.modKeyAlt = isAlt;
    event.modKeyShift = isShift;
    event.modKeyCtrl = isCtrl;
    eventManager.queueEvent(event);
}

void ApplicationEventsSystem::onMouseWheelEvent(EventManager &eventManager, glm::vec2 wheel, bool mouseLeft,
                                                bool mouseRight, bool isAlt, bool isCtrl,
                                                bool isShift) {
    auto &event = eventManager.createEvent<InputEvent>(InputEvent::MOUSEWHEEL);
    event.mouseWheel = wheel;
    event.mouseButtonLeft = mouseLeft;
    event.mouseButtonRight = mouseRight;
    event.modKeyAlt = isAlt;
    event.modKeyShift = isShift;
    event.modKeyCtrl = isCtrl;
    eventManager.queueEvent(event);
}
