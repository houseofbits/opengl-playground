
#include "Include.h"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "Events/InputEvent.h"
#include "Helper/GLDebugMessageCallback.h"
#include <glm/vec2.hpp>
#include "Helper/Time.h"
#include <iostream>

Window::Window(EventManager *eventManager) : eventManager(eventManager), sdlWindow(nullptr), viewportWidth(1024), viewportHeight(768), isFullScreen(false)
{
}

void Window::create()
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Failed to init SDL Video, error: %s", SDL_GetError());

        throw new Exception(1, "Failed to init SDL Video");
    }

    windowFlags = SDL_WINDOW_OPENGL;

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,8);

    sdlWindow = SDL_CreateWindow(
        "OpenGL-playground",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        viewportWidth,
        viewportHeight,
        windowFlags);

    if (sdlWindow == nullptr)
    {
        printf("Could not create window: %s", SDL_GetError());

        throw new Exception(2, "Could not create window");
    }

    int contextFlags = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &contextFlags);
    contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);   //3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GLContext Context = SDL_GL_CreateContext(sdlWindow);

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
    }

//     glEnable(GL_DEBUG_OUTPUT);
//     glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//     glDebugMessageCallback(GLDebugMessageCallback, nullptr);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    glEnable(GL_MULTISAMPLE);
//    glDisable(GL_MULTISAMPLE);

    eventManager->queueEvent(new WindowEvent(WindowEvent::Type::CREATE, this));

    // std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
//     int numViewports;
//     glGetIntegerv(GL_MAX_VIEWPORTS, &numViewports);
//     std::cout << "Num viewports: " << numViewports << std::endl;
//    int maxVertices;
//    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxVertices);
//    std::cout << "GS maximum number of vertices: " << maxVertices << std::endl;
}

void Window::destroy()
{
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

bool Window::pollEvents()
{
    setTime();

    SDL_Event sdl_event;

    SDL_Keymod modKeys = SDL_GetModState();
    bool isShift = modKeys & KMOD_SHIFT;
    bool isCtrl = modKeys & KMOD_CTRL;
    bool isAlt = modKeys & KMOD_ALT;
    bool leftButton = false;
    bool rightButton = false;

    while (SDL_PollEvent(&sdl_event) != 0)
    {
        if (sdl_event.type == SDL_QUIT)
        {
            return false;
        }
        else if (sdl_event.type == SDL_KEYDOWN)
        {
            switch (sdl_event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                return false;
                break;
            case SDLK_F11:
                isFullScreen = !isFullScreen;

                if (isFullScreen)
                {
                    SDL_SetWindowFullscreen(sdlWindow, windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                    SDL_GetWindowSize(sdlWindow, &viewportWidth, &viewportHeight);
                }
                else
                {
                    viewportWidth = 1024;
                    viewportHeight = 768;
                    SDL_SetWindowFullscreen(sdlWindow, windowFlags);
                }

                eventManager->queueEvent(new WindowEvent(WindowEvent::Type::RESIZE, this));
                break;
            }
        }

        if (sdl_event.button.button == SDL_BUTTON_LEFT)
        {
            leftButton = true;
        }
        if (sdl_event.button.button == SDL_BUTTON_RIGHT || sdl_event.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
        {
            rightButton = true;
        }

        switch (sdl_event.type)
        {
        case SDL_KEYDOWN:
            onKeyEvent(InputEvent::KEYDOWN, sdl_event.key.keysym.scancode, isAlt, isCtrl, isShift);
            break;
        case SDL_KEYUP:
            onKeyEvent(InputEvent::KEYUP, sdl_event.key.keysym.scancode, isAlt, isCtrl, isShift);
            break;
        case SDL_MOUSEMOTION:
            onMouseEvent(
                InputEvent::MOUSEMOVE,
                glm::vec2((float)sdl_event.motion.x, (float)sdl_event.motion.y),
                glm::vec2((float)sdl_event.motion.xrel, (float)sdl_event.motion.yrel),
                leftButton,
                rightButton,
                isAlt,
                isCtrl,
                isShift);
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseEvent(
                InputEvent::MOUSEDOWN,
                glm::vec2((float)sdl_event.motion.x, (float)sdl_event.motion.y),
                glm::vec2((float)sdl_event.motion.xrel, (float)sdl_event.motion.yrel),
                leftButton,
                rightButton,
                isAlt,
                isCtrl,
                isShift);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseEvent(
                InputEvent::MOUSEUP,
                glm::vec2((float)sdl_event.motion.x, (float)sdl_event.motion.y),
                glm::vec2((float)sdl_event.motion.xrel, (float)sdl_event.motion.yrel),
                leftButton,
                rightButton,
                isAlt,
                isCtrl,
                isShift);
            break;

        case SDL_TEXTINPUT:
            eventManager->queueEvent(new InputEvent(InputEvent::TEXENTER, std::string(sdl_event.text.text)));
            break;
        };
    }

    int numKeys;
    const Uint8 *state = SDL_GetKeyboardState(&numKeys);
    if (numKeys > 0)
    {
        for (int i = 0; i < numKeys; i++)
        {
            if (state[i] > 0)
            {
                onKeyEvent(InputEvent::KEYPRESS, i, isAlt, isCtrl, isShift);
            }
        }
    }

    return true;
}

void Window::doubleBuffer()
{
    SDL_GL_SwapWindow(sdlWindow);
}

void Window::onKeyEvent(InputEvent::EventType type, int keysym, bool isAlt, bool isCtrl, bool isShift)
{
    InputEvent *event = new InputEvent(type, keysym);
    event->modKeyAlt = isAlt;
    event->modKeyShift = isShift;
    event->modKeyCtrl = isCtrl;
    eventManager->queueEvent(event);
}

void Window::onMouseEvent(InputEvent::EventType type, glm::vec2 position, glm::vec2 motion, bool mouseLeft, bool mouseRight, bool isAlt, bool isCtrl, bool isShift)
{
    InputEvent *event = new InputEvent(type, position, motion, mouseLeft, mouseRight);
    event->modKeyAlt = isAlt;
    event->modKeyShift = isShift;
    event->modKeyCtrl = isCtrl;
    eventManager->queueEvent(event);
}

void Window::setTime()
{
    float currentTime = (float)SDL_GetTicks() / 1000.0f;

    Time::frameTime = currentTime - Time::timestamp;
    Time::timestamp = currentTime;
}