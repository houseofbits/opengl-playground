#pragma once
#include <SDL2/SDL.h>
#include "CoreV2/Events/EventManager.h"
#include "CoreV2/Events/InputEvent.h"

class Window
{
private:
    SDL_Window *sdlWindow;
    SDL_GLContext sdlGlContext;
    bool isFullScreen;
    int windowFlags;
    EventManager *eventManager;

public:
    int viewportWidth;
    int viewportHeight;

public:
    explicit Window(EventManager *eventManager);
    void create();
    bool pollEvents();
    void doubleBuffer();
    void destroy();
    void onKeyEvent(InputEvent::EventType type, int keysym, bool isAlt, bool isCtrl, bool isdShift);
    void onMouseEvent(InputEvent::EventType type, glm::vec2 position, glm::vec2 motion, bool mouseLeft, bool mouseRight, bool isAlt, bool isCtrl, bool isShift);
    static void setTime();
    SDL_Window* getSDLWindow() {
        return sdlWindow;
    }
    SDL_GLContext* getSDLContext() {
        return &sdlGlContext;
    }
};