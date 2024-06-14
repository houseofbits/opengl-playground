#pragma once

#include "../../CoreV2/API.h"
#include "../../Window.h"

class MainApplication : public Application {
public:
    MainApplication();

    Window m_Window;
    EventManager m_EventManager;

    void run() override;
    bool handleInputEvent(InputEvent *event);
};
