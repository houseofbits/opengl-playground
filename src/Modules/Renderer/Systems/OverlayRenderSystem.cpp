#include "OverlayRenderSystem.h"

OverlayRenderSystem::OverlayRenderSystem() : EntitySystem() {

}

void OverlayRenderSystem::process(EventManager &eventManager) {


}

void OverlayRenderSystem::initialize(ResourceManager &) {

}

void OverlayRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &OverlayRenderSystem::handleWindowEvent);
    eventManager.registerEventReceiver(this, &OverlayRenderSystem::handleEditorUIEvent);
}

void OverlayRenderSystem::handleWindowEvent(const WindowEvent *const event) {

}

void OverlayRenderSystem::handleEditorUIEvent(const EditorUIEvent *const event) {

}
