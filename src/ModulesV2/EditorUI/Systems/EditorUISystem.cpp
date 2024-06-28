#include "EditorUISystem.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../Events/EditorUIEvent.h"
#include <glm/gtc/type_ptr.hpp>

EditorUISystem::EditorUISystem() : EntitySystem(),
                                   m_isImUIInitialized(false),
                                   m_isDemoWindowVisible(false),
                                   m_isLightsWindowVisible(true),
                                   m_selectedLightEntity(-1),
                                   m_currentGizmoOperation(ImGuizmo::TRANSLATE),
                                   m_currentGizmoMode(ImGuizmo::WORLD) {
}

void EditorUISystem::registerComponent(Component *comp) {
    if (isOfType<TransformComponent>(comp)) {
        m_transformComponents[comp->m_EntityId()] = dynamic_cast<TransformComponent *>(comp);
    }
    if (isOfType<LightComponent>(comp)) {
        m_lightComponents[comp->m_EntityId()] = dynamic_cast<LightComponent *>(comp);
    }
    if (isOfType<CameraComponent>(comp)) {
        m_cameraComponents[comp->m_Id()] = dynamic_cast<CameraComponent *>(comp);
    }
}

void EditorUISystem::unregisterComponent(Component *comp) {
}

void EditorUISystem::process() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (m_isDemoWindowVisible) {
        ImGui::ShowDemoWindow(&m_isDemoWindowVisible);
    }

    processToolbar();

    if (m_isLightsWindowVisible) {
        processLightsWindow();
    }

    if (m_selectedLightEntity != -1) {
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

        ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::LOCAL;

        Camera *camera = findActiveCamera();
        TransformComponent *transform = m_transformComponents[m_selectedLightEntity];

        ImGuizmo::Manipulate(glm::value_ptr(camera->viewMatrix),
                             glm::value_ptr(camera->projectionMatrix),
                             (ImGuizmo::OPERATION) m_currentGizmoOperation,
                             (ImGuizmo::MODE) m_currentGizmoMode,
                             glm::value_ptr(transform->m_ModelMatrix));
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUISystem::initialize(ResourceManager *) {
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

void EditorUISystem::processToolbar() {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 30));
    ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("##TOOLBAR", nullptr, window_flags);
    ImGui::PopStyleVar();

    if(ImGui::Button("Save##SAVE_BUTTON")) {
        auto evt = new EditorUIEvent();
        evt->m_Type = EditorUIEvent::SAVE;
        m_EventManager->queueEvent(evt);
    }

    ImGui::SameLine();
    if (ImGui::Checkbox("Lights", &m_isLightsWindowVisible)) {
        auto evt = new EditorUIEvent();
        evt->m_Type = m_isLightsWindowVisible ? EditorUIEvent::EDITOR_ENABLED : EditorUIEvent::EDITOR_DISABLED;
        m_EventManager->queueEvent(evt);
    }

    if (m_selectedLightEntity != -1) {
        static const char *selected_transform_type_label = "Translate";

        ImGui::SameLine();
        ImGui::PushItemWidth(100);
        if (ImGui::BeginCombo("##TRANSFORM_TYPE", selected_transform_type_label)) {
            TransformComponent *transform = m_transformComponents[m_selectedLightEntity];

            if (transform->m_isTranslationEnabled) {
                if (ImGui::Selectable("Translate", m_currentGizmoOperation == ImGuizmo::TRANSLATE)) {
                    m_currentGizmoOperation = ImGuizmo::TRANSLATE;
                    selected_transform_type_label = "Translate";
                }
            }
            if (transform->m_isRotationEnabled) {
                if (ImGui::Selectable("Rotate", m_currentGizmoOperation == ImGuizmo::ROTATE)) {
                    m_currentGizmoOperation = ImGuizmo::ROTATE;
                    selected_transform_type_label = "Rotate";
                }
            }
            if (transform->m_isScalingEnabled) {
                if (ImGui::Selectable("Scale", m_currentGizmoOperation == ImGuizmo::SCALE)) {
                    m_currentGizmoOperation = ImGuizmo::SCALE;
                    selected_transform_type_label = "Scale";
                }
            }
            ImGui::EndCombo();
        }

        static const char *selected_transform_space_label = "WORLD";
        ImGui::SameLine();
        ImGui::PushItemWidth(100);
        if (ImGui::BeginCombo("##TRANSFORM_SPACE", selected_transform_space_label)) {
            if (ImGui::Selectable("WORLD", m_currentGizmoMode == ImGuizmo::WORLD)) {
                m_currentGizmoMode = ImGuizmo::WORLD;
                selected_transform_space_label = "WORLD";
            }


            if (ImGui::Selectable("LOCAL", m_currentGizmoMode == ImGuizmo::LOCAL)) {
                m_currentGizmoMode = ImGuizmo::LOCAL;
                selected_transform_space_label = "LOCAL";
            }

            ImGui::EndCombo();
        }
    }


    ImGui::End();
}

void EditorUISystem::processLightsWindow() {
    ImGui::SetNextWindowPos(ImVec2(0, 35));
    ImGui::SetNextWindowSize(ImVec2(300, 500));
    ImGui::Begin("Lights", &m_isLightsWindowVisible);
    ImGui::BeginListBox("##LIGHTS_LIST", ImVec2(ImGui::GetWindowWidth() - 15, 200));
    for (const auto &light: m_lightComponents) {
        Entity *e = m_EntityContext->getEntity(light.first);
        if (e != nullptr) {
            std::string name = e->m_Name;
            if (light.second->m_Type == LightComponent::SPOT) {
                name.append(" [SPOT LIGHT]");
            } else if (light.second->m_Type == LightComponent::OMNI) {
                name.append(" [OMNI LIGHT]");
            } else if (light.second->m_Type == LightComponent::DIRECT) {
                name.append(" [DIRECT LIGHT]");
            }

            if (ImGui::Selectable(name.c_str(), m_selectedLightEntity == light.first)) {
                m_selectedLightEntity = (int) light.first;
            }
        }
    }
    ImGui::EndListBox();

    if (m_selectedLightEntity != -1) {
        LightComponent *light = m_lightComponents[m_selectedLightEntity];
        if (light != nullptr) {
            Entity *e = m_EntityContext->getEntity(light->m_EntityId.id());

            std::string title = "Light: " + e->m_Name;
            ImGui::SeparatorText(title.c_str());

            ImGui::InputText("Name", &e->m_Name);
            ImGui::ColorEdit3("Color##LIGHT_COLOR", (float *) &light->m_Color);
            ImGui::InputFloat("Intensity##LIGHT_INTENSITY", &light->m_Intensity, 0.1f, 1.0f, "%.1f");
            ImGui::InputFloat("Attenuation##LIGHT_ATTENUATION", &light->m_Attenuation, 0.1f, 1.0f, "%.1f");
            ImGui::InputFloat("Beam angle##LIGHT_BEAM_ANGLE", &light->m_beamAngle, 0.5f, 1.0f, "%.0f");

            std::string path = light->m_Projection().m_Path;
            ImGui::InputText("Projector", &path);
        }

        ImGui::SeparatorText("Transform");

        processTransformComponent();
    }

    ImGui::End();
}

void EditorUISystem::processTransformComponent() {
    TransformComponent *transform = m_transformComponents[m_selectedLightEntity];

    float matrixTranslation[3], matrixRotation[3], matrixScale[3];

    ImGuizmo::DecomposeMatrixToComponents(&transform->m_ModelMatrix[0][0], matrixTranslation, matrixRotation, matrixScale);
    if (transform->m_isTranslationEnabled) {
        ImGui::InputFloat3("Translation", matrixTranslation);
    }
    if (transform->m_isRotationEnabled) {
        ImGui::InputFloat3("Rotation", matrixRotation);
    }
    if (transform->m_isScalingEnabled) {
        ImGui::InputFloat3("Scaling", matrixScale);
    }
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, &transform->m_ModelMatrix[0][0]);
}

Camera *EditorUISystem::findActiveCamera() {
    for (const auto &camera: m_cameraComponents) {
        if (camera.second->m_isActive) {
            return &camera.second->m_Camera;
        }
    }

    return nullptr;
}
