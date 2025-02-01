#include "EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_opengl3.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../Helpers/TexturePreviewHelper.h"

EditorUISystem::EditorUISystem() : EntitySystem(),
                                   m_transformGizmo(),
                                   m_ResourceManager(nullptr),
                                   m_isImUIInitialized(false),
                                   m_MainToolbarUI(this),
                                   m_EditWindowUI(this),
                                   m_MaterialEditWindowUI(this),
                                   m_selectedEntityId(0),
                                   m_isEditorModeEnabled(false),
                                   m_componentEditors(),
                                   m_activeCameraHelper() {
    m_editorCameraComponentRegistry = useComponentRegistry<EditorCameraComponent>();
    m_cameraComponentRegistry = useComponentRegistry<CameraComponent>();
}

void EditorUISystem::process(EventManager &eventManager) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    processDockSpaceWindow();

    m_MainToolbarUI.process();
    if (m_isEditorModeEnabled) {
        m_EditWindowUI.process();
    }

    m_MaterialEditWindowUI.process();

    TexturePreviewHelper::process();

    if (m_isEditorModeEnabled) {
        Camera *camera = m_activeCameraHelper.find(*m_EntityContext);
        if (camera != nullptr) {
            m_transformGizmo.processGizmo(*m_EntityContext, m_selectedEntityId, *camera);
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUISystem::initialize(ResourceManager &manager, EventManager &) {
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
    eventManager.registerEventReceiver(this, &EditorUISystem::handleSystemEvent);
    eventManager.registerEventReceiver(this, &EditorUISystem::handleRawSDLEvent);
    eventManager.registerEventReceiver(this, &EditorUISystem::handleCameraActivationEvent);
}

void EditorUISystem::handleSystemEvent(const SystemEvent &event) {
    if (event.eventType == SystemEvent::WINDOW_CONTEXT_CREATED) {
        ImGui_ImplSDL2_InitForOpenGL(event.window->getSDLWindow(), event.window->getSDLContext());
        ImGui_ImplOpenGL3_Init("#version 410");
        m_isImUIInitialized = true;
    } else if (event.eventType == SystemEvent::REQUEST_EDITOR_MODE) {
        m_isEditorModeEnabled = true;
    } else if (event.eventType == SystemEvent::REQUEST_GAME_MODE) {
        m_isEditorModeEnabled = false;
    }
}

void EditorUISystem::handleRawSDLEvent(const RawSDLEvent &event) {
    if (m_isImUIInitialized) {
        ImGui_ImplSDL2_ProcessEvent(&event.sdlEvent);
    }
}

TransformComponent *EditorUISystem::getSelectedTransformComponent() {
    if (m_EditWindowUI.isTransformComponentSelected()) {
        return m_EntityContext->getEntityComponent<TransformComponent>(m_selectedEntityId);
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
