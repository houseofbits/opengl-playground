#include "EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_opengl3.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include <glm/gtc/type_ptr.hpp>

EditorUISystem::EditorUISystem() : EntitySystem(),
                                   m_lightComponents(),
                                   m_transformComponents(),
                                   m_cameraComponents(),
                                   m_ResourceManager(nullptr),
                                   m_meshComponents(),
                                   m_environmentProbeComponents(),
                                   m_isImUIInitialized(false),
                                   m_isDemoWindowVisible(false),
                                   m_MainToolbarUI(this),
                                   m_EditWindowUI(this) {
}

void EditorUISystem::registerComponent(Component *comp) {
    if (isOfType<StaticMeshComponent>(comp)) {
        m_meshComponents[comp->m_EntityId()] = dynamic_cast<StaticMeshComponent *>(comp);
    }
    if (isOfType<TransformComponent>(comp)) {
        m_transformComponents[comp->m_EntityId()] = dynamic_cast<TransformComponent *>(comp);
    }
    if (isOfType<LightComponent>(comp)) {
        m_lightComponents[comp->m_EntityId()] = dynamic_cast<LightComponent *>(comp);
    }
    if (isOfType<CameraComponent>(comp)) {
        m_cameraComponents[comp->m_EntityId()] = dynamic_cast<CameraComponent *>(comp);
    }
    if (isOfType<EnvironmentProbeComponent>(comp)) {
        m_environmentProbeComponents[comp->m_EntityId()] = dynamic_cast<EnvironmentProbeComponent *>(comp);
    }
}

void EditorUISystem::unregisterComponent(Component *comp) {
    m_meshComponents.erase(comp->m_EntityId.id());
    m_transformComponents.erase(comp->m_EntityId.id());
    m_cameraComponents.erase(comp->m_EntityId.id());
    m_lightComponents.erase(comp->m_EntityId.id());
    m_environmentProbeComponents.erase(comp->m_EntityId.id());
}

void EditorUISystem::process() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (m_isDemoWindowVisible) {
        ImGui::ShowDemoWindow(&m_isDemoWindowVisible);
    }

    m_MainToolbarUI.process();
    if (m_MainToolbarUI.m_isEditWindowVisible) {
        m_EditWindowUI.process();
    }

    if (m_EditWindowUI.isTransformComponentSelected()) {
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

        Camera *camera = findActiveCamera();
        TransformComponent *transform = m_transformComponents[m_EditWindowUI.m_selectedEntity];

        static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };

        ImGuizmo::Manipulate(glm::value_ptr(camera->viewMatrix),
                             glm::value_ptr(camera->projectionMatrix),
                             (ImGuizmo::OPERATION) m_MainToolbarUI.m_currentGizmoOperation,
                             (ImGuizmo::MODE) m_MainToolbarUI.m_currentGizmoMode,
                             glm::value_ptr(transform->m_ModelMatrix),
                             nullptr,
                             nullptr,
                             bounds
                             );
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUISystem::initialize(ResourceManager *manager) {
    m_ResourceManager = manager;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsLight();
}

void EditorUISystem::registerEventHandlers(EventManager *eventManager) {
    eventManager->registerEventReceiver(this, &EditorUISystem::handleWindowEvent);
    eventManager->registerEventReceiver(this, &EditorUISystem::handleRawSDLEvent);
}

bool EditorUISystem::handleWindowEvent(WindowEvent *event) {
    if (event->eventType == WindowEvent::OPENGL_CONTEXT_CREATED) {
        ImGui_ImplSDL2_InitForOpenGL(event->window->getSDLWindow(), event->window->getSDLContext());
        ImGui_ImplOpenGL3_Init("#version 450");
        m_isImUIInitialized = true;
    }
    return true;
}

bool EditorUISystem::handleRawSDLEvent(RawSDLEvent *event) {
    if (m_isImUIInitialized) {
        ImGui_ImplSDL2_ProcessEvent(&event->sdlEvent);
    }

    return true;
}

Camera *EditorUISystem::findActiveCamera() {
    for (const auto &camera: m_cameraComponents) {
        if (camera.second->m_isActive) {
            return &camera.second->m_Camera;
        }
    }

    return nullptr;
}

TransformComponent *EditorUISystem::getSelectedTransformComponent() {
    if (m_EditWindowUI.isTransformComponentSelected()
        && m_transformComponents.find(m_EditWindowUI.m_selectedEntity) != m_transformComponents.end()) {
        return m_transformComponents[m_EditWindowUI.m_selectedEntity];
    }
    return nullptr;
}
