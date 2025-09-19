#pragma once

#include "../../../Core/API.h"
#include <SDL2/SDL.h>

class RawSDLEvent : public BaseEvent {
TYPE_DEFINITION(RawSDLEvent);

public:
    explicit RawSDLEvent(SDL_Event e) : sdlEvent(e) {}

    SDL_Event sdlEvent;
};