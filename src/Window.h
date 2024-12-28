#pragma once
#include <SDL2/SDL.h>
#include "Core/Events/EventManager.h"
#include "Core/Events/EventManager_V2.h"
#include "Modules/Application/Events/InputEvent.h"

class Window {
private:
    SDL_Window *sdlWindow;
    SDL_GLContext sdlGlContext;
    bool isFullScreen;
    int windowFlags;

public:
    int viewportWidth;
    int viewportHeight;

public:
    explicit Window();

    void create(EventManager &);

    void doubleBuffer() const;

    void destroy() const;

    SDL_Window *getSDLWindow() const {
        return sdlWindow;
    }

    SDL_GLContext *getSDLContext() {
        return &sdlGlContext;
    }
};
