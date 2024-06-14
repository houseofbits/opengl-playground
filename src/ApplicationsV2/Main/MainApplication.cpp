#include "MainApplication.h"

MainApplication::MainApplication() : Application(), m_EventManager(), m_Window(&m_EventManager) {
    m_EventManager.registerEventReceiver(this, &MainApplication::handleInputEvent);
}

void MainApplication::run() {
//    m_Window.create();
//
//    while (true) {
//        if (!m_Window.pollEvents() || !m_EventManager.processEvents()) {
//            break;
//        }
//        m_EntityContext.processSystems();
//    }
//
//    m_Window.destroy();
}

bool MainApplication::handleInputEvent(InputEvent *const event) {
    if (event->type == InputEvent::KEYDOWN) {
        std::cout << event->keyCode << std::endl;
    }

    return true;
}