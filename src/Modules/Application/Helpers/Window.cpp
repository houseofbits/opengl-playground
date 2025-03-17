#include "Window.h"
#include "../Events/InputEvent.h"
#include "Exception.h"
#include "../Events/RawSDLEvent.h"
#include "../Events/SystemEvent.h"
#include "../../../Core/Helper/GLDebugMessageCallback.h"
#include <GL/glew.h>
#include <iostream>

Window::Window() : sdlWindow(nullptr),
                   sdlGlContext(),
                   isFullScreen(false),
                   windowFlags(),
                   viewportWidth(1920),
                   viewportHeight(1080) {
}

void Window::create(EventManager &eventManager) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Failed to init SDL Video, error: %s", SDL_GetError());

        throw Exception(1, "Failed to init SDL Video");
    }

    windowFlags = SDL_WINDOW_OPENGL;

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    sdlWindow = SDL_CreateWindow(
        "OpenGL-playground",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        viewportWidth,
        viewportHeight,
        windowFlags);

    eventManager.queueEvent<SystemEvent>(SystemEvent::Type::WINDOW_CREATED, this);

    if (sdlWindow == nullptr) {
        printf("Could not create window: %s", SDL_GetError());

        throw Exception(2, "Could not create window");
    }

    // int contextFlags = 0;
    // SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &contextFlags);
    // contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); //3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); //2
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  //MacOS
    //    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );    //MacOS

    sdlGlContext = SDL_GL_CreateContext(sdlWindow);

    // std::cout<<SDL_GetError()<<std::endl;

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cout << "Context creation error" << std::endl;
    }

    //    SDL_GL_MakeCurrent(sdlWindow, sdlGlContext);

    // glEnable(GL_DEBUG_OUTPUT);
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    // glDebugMessageCallback(GLDebugMessageCallback, nullptr);    //Supported only on gl >=4.3 (Not on MacOS)

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    glEnable(GL_MULTISAMPLE);
    //    glDisable(GL_MULTISAMPLE);


    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    //    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    //     int numViewports;
    //     glGetIntegerv(GL_MAX_VIEWPORTS, &numViewports);
    //     std::cout << "Num viewports: " << numViewports << std::endl;
    //    int maxVertices;
    //    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxVertices);
    //    std::cout << "GS maximum number of vertices: " << maxVertices << std::endl;

    eventManager.queueEvent<SystemEvent>(SystemEvent::Type::WINDOW_CONTEXT_CREATED, this);

    // std::cout<<"Window created"<<std::endl;
}

void Window::destroy() const {
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void Window::doubleBuffer() const {
    SDL_GL_SwapWindow(sdlWindow);
}
