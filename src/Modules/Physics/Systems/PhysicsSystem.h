#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Components/CharacterControllerComponent.h"
#include "../Resources/PhysicsResource.h"
#include "../Helpers/DebugRendererImpl.h"

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleEditorUIEvent(const EditorUIEvent &);

private:
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
    DebugRendererImpl m_debugRenderer;
};
