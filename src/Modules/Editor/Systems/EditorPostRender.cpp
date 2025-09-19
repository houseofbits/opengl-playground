#include "EditorPostRender.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_opengl3.h"
#include "../../Physics/Components/PhysicsShapeComponent.h"

EditorPostRender::EditorPostRender() : EntitySystem(), m_wireframeRenderer() {
    m_physicsBodiesRegistry = useEntityUniqueComponentRegistry<PhysicsBodyComponent>();
}

void EditorPostRender::initialize(ResourceManager &, EventManager &) {
    m_wireframeRenderer.initialize();
}

void EditorPostRender::process(EventManager &) {
    if (!m_isImUIInitialized) {
        return;
    }

    Camera *camera = m_activeCameraHelper.find(*m_EntityContext);
    if (camera != nullptr && m_isEditorModeEnabled && m_doREnderWireframes) {
        m_wireframeRenderer.setCamera(*camera);

        for (const auto &[id, component]: m_physicsBodiesRegistry->container()) {
            auto e = m_EntityContext->getEntity(id);
            if (!e) {
                continue;
            }

            auto transformComponent = e->getComponent<TransformComponent>();
            auto transform = glm::mat4(1.0);
            if (transformComponent) {
                transform = transformComponent->getWorldTransform();
            }

            if (auto shapeComponents = e->getAllComponents<PhysicsShapeComponent>(); shapeComponents.size() > 0) {
                for (const auto &c: shapeComponents) {
                    auto m = c->getWorldTransform(transform);
                    if (c->m_type == PhysicsShapeComponent::TYPE_BOX) {
                        m_wireframeRenderer.renderCube(m, glm::vec4(0, 0, 1, 1));
                    }
                    if (c->m_type == PhysicsShapeComponent::TYPE_SPHERE) {
                        m_wireframeRenderer.renderSphere(m, glm::vec4(0, 0, 1, 1));
                    }
                    if (c->m_type == PhysicsShapeComponent::TYPE_MESH && c->m_meshResource.isReady()) {
                        m_wireframeRenderer.render(m, c->m_meshResource().m_wireModel, glm::vec4(0, 0, 1, 1));
                    }
                }
            }
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorPostRender::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &EditorPostRender::handleSystemEvent);
    eventManager.registerEventReceiver(this, &EditorPostRender::handleCameraActivationEvent);
    eventManager.registerEventReceiver(this, &EditorPostRender::handleEditorUIEvent);
}

void EditorPostRender::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::WINDOW_CONTEXT_CREATED) {
        m_isImUIInitialized = true;
    } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
        m_isEditorModeEnabled = true;
    } else if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        m_isEditorModeEnabled = false;
    }
}

void EditorPostRender::handleEditorUIEvent(const EditorUIEvent &event) {
    if (event.m_Type == EditorUIEvent::SHOW_PHYSICS_SHAPES) {
        m_doREnderWireframes = true;
    }

    if (event.m_Type == EditorUIEvent::HIDE_PHYSICS_SHAPES) {
        m_doREnderWireframes = false;
    }
}
