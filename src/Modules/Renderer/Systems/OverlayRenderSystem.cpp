#include "OverlayRenderSystem.h"

OverlayRenderSystem::OverlayRenderSystem() : EntitySystem() {

}

void OverlayRenderSystem::process(EventManager &eventManager) {


}

void OverlayRenderSystem::initialize(ResourceManager &) {

}

void OverlayRenderSystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &OverlayRenderSystem::handleSystemEvent);
    eventManager.registerEventReceiver(this, &OverlayRenderSystem::handleEditorUIEvent);
}

void OverlayRenderSystem::handleSystemEvent(const SystemEvent & event) {

}

void OverlayRenderSystem::handleEditorUIEvent(const EditorUIEvent & event) {

}
