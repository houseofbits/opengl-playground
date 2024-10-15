#include "EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_opengl3.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../Physics/Components/CharacterControllerComponent.h"
#include "../../Physics/Components/PhysicsBodyComponent.h"
#include "../../Physics/Components/PhysicsHingeJointComponent.h"
#include "../UI/TexturePreviewHelper.h"
#include "../../Physics/Components/PhysicsFixedJointComponent.h"
#include "../../Physics/Components/PhysicsSliderJointComponent.h"

EditorUISystem::EditorUISystem() : EntitySystem(),
                                   m_transformGizmo(),
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
    usesComponent<CharacterControllerComponent>();
    usesComponent<PhysicsBodyComponent>();
    usesComponent<PhysicsHingeJointComponent>();
    usesComponent<PhysicsFixedJointComponent>();
    usesComponent<PhysicsSliderJointComponent>();
}

void EditorUISystem::process(EventManager &eventManager) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    processDockSpaceWindow();

    m_MainToolbarUI.process();
    if (m_MainToolbarUI.m_isEditWindowVisible) {
        m_EditWindowUI.process();
    }

    m_MaterialEditWindowUI.process();

    TexturePreviewHelper::process();

    if (!m_MainToolbarUI.m_isSimulationEnabled) {
        Camera *camera = findActiveCamera();
        assert(camera != nullptr);
        m_transformGizmo.processGizmo(*m_EntityContext, m_EditWindowUI.m_selectedEntityId, *camera);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUISystem::initialize(ResourceManager &manager) {
    m_ResourceManager = &manager;


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//    ImGui_ImplSDL2_Init();
//    ImGui_ImplOpenGL3_Init();
//

//    ImGui::StyleColorsLight();

//    ImGuiStyle& style = ImGui::GetStyle();
//    style.WindowPadding.x = 0;
//    style.WindowPadding.y = 0;
}

void EditorUISystem::registerEventHandlers(EventManager &eventManager) {
    eventManager.registerEventReceiver(this, &EditorUISystem::handleWindowEvent);
    eventManager.registerEventReceiver(this, &EditorUISystem::handleRawSDLEvent);
}

void EditorUISystem::handleWindowEvent(const WindowEvent &event) {
    if (event.eventType == WindowEvent::OPENGL_CONTEXT_CREATED) {
        ImGui_ImplSDL2_InitForOpenGL(event.window->getSDLWindow(), event.window->getSDLContext());
        ImGui_ImplOpenGL3_Init("#version 410");
        m_isImUIInitialized = true;
    }
}

void EditorUISystem::handleRawSDLEvent(const RawSDLEvent &event) {
    if (m_isImUIInitialized) {
        ImGui_ImplSDL2_ProcessEvent(&event.sdlEvent);
    }
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
        return getComponent<TransformComponent>(m_EditWindowUI.m_selectedEntityId);
    }
    return nullptr;
}

void EditorUISystem::openMaterialEditor(ResourceHandle<MaterialResource> &handle) {
    if (handle.isValid()) {
        m_MaterialEditWindowUI.open(handle);
    }
}

void EditorUISystem::processDockSpaceWindow() {
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
    ImGui::Begin("Dockspace Window", nullptr, windowFlags);
    ImGui::PopStyleVar(2);
    ImGuiID dockspaceID = ImGui::GetID("MainDockspace");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}
