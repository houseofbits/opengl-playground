#include "EditorUISystem.h"
#include "../UI/TexturePreviewHelper.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_opengl3.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include <glm/gtc/type_ptr.hpp>

EditorUISystem::EditorUISystem() : EntitySystem(),
                                   m_ResourceManager(nullptr),
                                   m_isImUIInitialized(false),
                                   m_isDemoWindowVisible(false),
                                   m_MainToolbarUI(this),
                                   m_EditWindowUI(this),
                                   m_MaterialEditWindowUI(this) {
    usesComponent<StaticMeshComponent>();
    usesComponent<LightComponent>();
    usesComponent<TransformComponent>();
    usesComponent<CameraComponent>();
    usesComponent<EnvironmentProbeComponent>();
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

    m_MaterialEditWindowUI.process();

    TexturePreviewHelper::process();

    if (m_EditWindowUI.isTransformComponentSelected()) {
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

        Camera *camera = findActiveCamera();
        auto *transform = getComponent<TransformComponent>(m_EditWindowUI.m_selectedEntity);
        assert(transform != nullptr);

        static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };

        ImGuizmo::Manipulate(glm::value_ptr(camera->viewMatrix),
                             glm::value_ptr(camera->projectionMatrix),
                             (ImGuizmo::OPERATION) m_MainToolbarUI.m_currentGizmoOperation,
                             (ImGuizmo::MODE) m_MainToolbarUI.m_currentGizmoMode,
                             glm::value_ptr(transform->m_ModelMatrix),
                             nullptr,
                             nullptr,
                             m_EditWindowUI.m_isBoundsTransformAllowed ? bounds : nullptr
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
        ImGui_ImplOpenGL3_Init("#version 410");
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
    auto *c = findComponent<CameraComponent>([](CameraComponent *camera) {
        return camera->m_isActive;
    });

    if (c != nullptr) {
        return &c->m_Camera;
    }

    return nullptr;
}

TransformComponent *EditorUISystem::getSelectedTransformComponent() {
    if (m_EditWindowUI.isTransformComponentSelected()) {
        return getComponent<TransformComponent>(m_EditWindowUI.m_selectedEntity);
    }
    return nullptr;
}

void EditorUISystem::openMaterialEditor(ResourceHandle<MaterialResource>& handle) {
    if (handle.isValid()) {
        m_MaterialEditWindowUI.open(handle);
    }
}