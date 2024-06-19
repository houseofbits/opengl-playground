#pragma once

#include "../../CoreV2/API.h"
#include "../../CoreV2/Resources/ResourceManager.h"
#include "../../Window.h"

class MainApplication : public Application {
public:
    MainApplication();

    Window m_Window;


    void run() override;
    void initialize(const std::string& entityDefinitionFileName);
    bool handleInputEvent(InputEvent *event);
};
