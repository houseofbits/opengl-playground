#include "OverlayRenderSystem.h"

OverlayRenderSystem::OverlayRenderSystem() : EntitySystem() {

}

void OverlayRenderSystem::process() {



}

void OverlayRenderSystem::initialize(ResourceManager *) {

}

void OverlayRenderSystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &OverlayRenderSystem::handleWindowEvent);
    eventManager->registerEventReceiver(this, &OverlayRenderSystem::handleEditorUIEvent);
}

bool OverlayRenderSystem::handleWindowEvent(WindowEvent *event) {
    return true;
}

bool OverlayRenderSystem::handleEditorUIEvent(EditorUIEvent *event) {
    return true;
}
