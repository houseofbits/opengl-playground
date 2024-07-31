#include "EditWindowUI.h"

#include "../../../Core/Events/EntityCreationEvent.h"
#include "../../../SourceLibs/imgui/ImGuiFileDialog.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/imgui_stdlib.h"
#include "../Systems/EditorUISystem.h"
#include "FileDialogHelper.h"
#include "TextPromptHelper.h"
#include "TexturePreviewHelper.h"

#include <utility>

std::vector<std::string> ENTITY_CREATION_OPTIONS = {
        "SpotLight",
        "OmniLight",
        "DirectLight",
        "StaticMesh",
        "EnvironmentProbe"};

std::map<LightComponent::Type, std::string> LIGHT_TYPE_NAME_MAP = {
        {LightComponent::SPOT, "SPOT"},
        {LightComponent::OMNI, "OMNI"},
        {LightComponent::DIRECT, "DIRECT"}};

std::map<int, std::string> SHADOW_MAP_RESOLUTION = {
        {64, "64x64"},
        {128, "128x128"},
        {256, "256x256"},
        {512, "512x512"},
        {1024, "1024x1024"},
        {2048, "2048x2048"},
};

EditWindowUI::EditWindowUI(EditorUISystem *editor) : m_selectedEntity(-1),
                                                     m_EditorUISystem(editor),
                                                     m_lightProjectorPath(),
                                                     m_isBoundsTransformAllowed(false),
                                                     m_meshModelPath(),
                                                     m_meshMaterialPath(),
                                                     m_selectedEntityCreationType(0),
                                                     m_isLightEntitiesListed(true),
                                                     m_isStaticMeshEntitiesListed(true),
                                                     m_isCameraEntitiesListed(true),
                                                     m_isProbeEntitiesListed(true),
                                                     m_isNewMaterialPromptVisible(false) {
}

void EditWindowUI::process() {
    ImGui::SetNextWindowPos(ImVec2(0, 25));
    ImGui::SetNextWindowSize(ImVec2(300, 600));
    if (ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoCollapse)) {

        ImGui::PushItemWidth(100);
        if (ImGui::BeginCombo("##ENTITY_TYPE", ENTITY_CREATION_OPTIONS[m_selectedEntityCreationType].c_str())) {
            for (int i = 0; i < ENTITY_CREATION_OPTIONS.size(); i++) {
                if (ImGui::Selectable(ENTITY_CREATION_OPTIONS[i].c_str(), m_selectedEntityCreationType == i)) {
                    m_selectedEntityCreationType = i;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("Create")) {
            sendEntityCreationEvent(ENTITY_CREATION_OPTIONS[m_selectedEntityCreationType], "NEW ENTITY");
        }

        if (m_selectedEntity >= 0) {
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                sendEntityRemovalEvent();
                m_selectedEntity = -1;
            }
        }

        if (ImGui::BeginCombo("##ENTITY_FILTER", "Filter")) {
            ImGui::Checkbox("LIGHTS", &m_isLightEntitiesListed);
            ImGui::Checkbox("STATIC MESHES", &m_isStaticMeshEntitiesListed);
            ImGui::Checkbox("CAMERAS", &m_isCameraEntitiesListed);
            ImGui::Checkbox("PROBES", &m_isProbeEntitiesListed);

            m_selectedEntity = -1;

            ImGui::EndCombo();
        }

        processEntitiesList();

        Entity *e = m_EditorUISystem->m_EntityContext->getEntity(m_selectedEntity);
        if (e != nullptr) {
            ImGui::InputText("Name", &e->m_Name);

            processEditLightComponent();
            processEditMeshComponent();
            processEditCameraComponent();
            processEditProbeComponent();
            processEditTransformComponent();
        }

        ImGui::End();
    }
}

void EditWindowUI::processEntitiesList() {
    ImGui::BeginListBox("##ENTITY_LIST", ImVec2(ImGui::GetWindowWidth() - 15, 200));

    if (m_isLightEntitiesListed) {
        for (const auto &light: m_EditorUISystem->getComponentContainer<LightComponent>()) {
            Entity *e = m_EditorUISystem->m_EntityContext->getEntity(light.first);
            if (e != nullptr) {
                std::string name = light.second->getListName(e);
                if (ImGui::Selectable(name.c_str(), m_selectedEntity == light.first)) {
                    m_selectedEntity = (int) light.first;
                    m_lightProjectorPath = light.second->m_Projection().m_Path;
                    m_isBoundsTransformAllowed = false;
                }
            }
        }
    }

    if (m_isStaticMeshEntitiesListed) {
        for (const auto &mesh: m_EditorUISystem->getComponentContainer<StaticMeshComponent>()) {
            Entity *e = m_EditorUISystem->m_EntityContext->getEntity(mesh.first);
            if (e != nullptr) {
                std::string name = mesh.second->getListName(e);
                if (ImGui::Selectable(name.c_str(), m_selectedEntity == mesh.first)) {
                    m_selectedEntity = (int) mesh.first;
                    m_meshModelPath = mesh.second->m_Mesh().m_Path;
                    m_meshMaterialPath = mesh.second->m_Material().m_Path;
                    m_isBoundsTransformAllowed = false;
                }
            }
        }
    }

    if (m_isCameraEntitiesListed) {
        for (const auto &camera: m_EditorUISystem->getComponentContainer<CameraComponent>()) {
            Entity *e = m_EditorUISystem->m_EntityContext->getEntity(camera.first);
            if (e != nullptr) {
                std::string name = camera.second->getListName(e);
                if (ImGui::Selectable(name.c_str(), m_selectedEntity == camera.first)) {
                    m_selectedEntity = (int) camera.first;
                    m_isBoundsTransformAllowed = false;
                }
            }
        }
    }

    if (m_isProbeEntitiesListed) {
        for (const auto &probe: m_EditorUISystem->getComponentContainer<EnvironmentProbeComponent>()) {
            Entity *e = m_EditorUISystem->m_EntityContext->getEntity(probe.first);
            if (e != nullptr) {
                std::string name = probe.second->getListName(e);
                if (ImGui::Selectable(name.c_str(), m_selectedEntity == probe.first)) {
                    m_selectedEntity = (int) probe.first;
                    m_isBoundsTransformAllowed = true;
                }
            }
        }
    }

    ImGui::EndListBox();
}

void EditWindowUI::processEditLightComponent() {
    auto *light = m_EditorUISystem->getComponent<LightComponent>(m_selectedEntity);
    if (light == nullptr) {
        return;
    }

    ImGui::SeparatorText("Light");

    ImGui::Checkbox("Is enabled", &light->m_isEnabled);

    if (ImGui::BeginCombo("Type##LIGHT_TYPE", LIGHT_TYPE_NAME_MAP[light->m_Type].c_str())) {
        for (const auto &lightName: LIGHT_TYPE_NAME_MAP) {
            if (ImGui::Selectable(lightName.second.c_str(), light->m_Type == lightName.first)) {
                light->m_Type = lightName.first;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::ColorEdit3("Color##LIGHT_COLOR", (float *) &light->m_Color);
    ImGui::InputFloat("Intensity##LIGHT_INTENSITY", &light->m_Intensity, 0.1f, 1.0f, "%.1f");
    ImGui::InputFloat("Attenuation##LIGHT_ATTENUATION", &light->m_Attenuation, 0.1f, 1.0f, "%.1f");
    if (light->m_Type == LightComponent::DIRECT) {
        ImGui::InputFloat("Radius##LIGHT_DIRECT_RADIUS", &light->m_Radius, 0.1f, 0.5f, "%.1f");
    } else if (light->m_Type == LightComponent::SPOT) {
        ImGui::InputFloat("Beam angle##LIGHT_BEAM_ANGLE", &light->m_beamAngle, 0.5f, 1.0f, "%.0f");
    }

    if (FileInput("ChooseLightProjectorFile", "Choose image file", ".png,.tga", "Projector", m_lightProjectorPath)) {
        m_EditorUISystem->m_ResourceManager->request(light->m_Projection, m_lightProjectorPath);
    }

    TexturePreviewHelper::texturePreview(light->m_Projection);

    ImGui::Checkbox("Cast shadows", &light->m_doesCastShadows);
    if (light->m_doesCastShadows) {
        if (ImGui::BeginCombo("Shadow size##SHADOW_RESOLUTION", SHADOW_MAP_RESOLUTION[light->m_shadowResolution].c_str())) {
            for (const auto &resolution: SHADOW_MAP_RESOLUTION) {
                if (ImGui::Selectable(resolution.second.c_str(), light->m_shadowResolution == resolution.first)) {
                    light->resizeShadowMaps(resolution.first);
                }
            }
            ImGui::EndCombo();
        }

        ImGui::InputFloat("Bias", &light->m_shadowBias, 0.001f, 0.01f, "%.5f");

        // for (int i = 0; i < light->m_ShadowMaps.size(); ++i) {
        //     std::string buttonName = "Preview " + std::to_string(i + 1);
        //     if (ImGui::Button(buttonName.c_str())) {
        //         //                std::cout<<light->m_ShadowMaps[i]->get().m_Path<<std::endl;
        //         m_isPreviewWindowOpen = true;
        //         m_previewTextureId = light->m_ShadowMaps[i]->get().m_textureRenderTarget.textureId;
        //         m_previewTextureSize = light->m_ShadowMaps[i]->get().m_textureRenderTarget.width;
        //     }
        // }
    }
}

void EditWindowUI::processEditMeshComponent() {
    auto *mesh = m_EditorUISystem->getComponent<StaticMeshComponent>(m_selectedEntity);
    if (mesh == nullptr) {
        return;
    }

    ImGui::SeparatorText("Mesh");

    if (ImGui::BeginCombo("Render##RENDER_TYPE", mesh->m_TargetRenderNameMap[mesh->m_targetRenderer].c_str())) {
        for (const auto &renderType: mesh->m_TargetRenderNameMap) {
            if (ImGui::Selectable(renderType.second.c_str(), mesh->m_targetRenderer == renderType.first)) {
                mesh->m_targetRenderer = renderType.first;
            }
        }
        ImGui::EndCombo();
    }

    if (FileInput("ChooseModelFile", "Choose GLTF Model file", ".gltf", "Model", m_meshModelPath)) {
        m_EditorUISystem->m_ResourceManager->request(mesh->m_Mesh, m_meshModelPath);
    }

    if (FileInput("ChooseMaterialFile", "Choose JSON Material file", ".json", "Material", m_meshMaterialPath)) {
        m_EditorUISystem->m_ResourceManager->request(mesh->m_Material, m_meshMaterialPath);
    }

    if (mesh->m_Material.isValid()) {
        if (ImGui::Button("Edit material")) {
            m_EditorUISystem->openMaterialEditor(mesh->m_Material);
        }
    }

    if(TextPromptHelper::textPrompt("New material", "New material", "Filename")) {
        m_EditorUISystem->m_ResourceManager->request(mesh->m_Material,
            "data/materials/" + TextPromptHelper::m_InputValue);

        m_meshMaterialPath = mesh->m_Material().m_Path;
    }
}

void EditWindowUI::processEditTransformComponent() {
    auto *transform = m_EditorUISystem->getComponent<TransformComponent>(m_selectedEntity);
    if (transform == nullptr) {
        return;
    }

    ImGui::SeparatorText("Transform");

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

void EditWindowUI::processEditCameraComponent() {
    auto *camera = m_EditorUISystem->getComponent<CameraComponent>(m_selectedEntity);
    if (camera == nullptr) {
        return;
    }

    ImGui::SeparatorText("Camera");

    float fov = camera->m_Camera.fieldOfView;
    if (ImGui::InputFloat("FOV##CAMERA_FOV", &fov, 1.0f, 5.0f, "%.0f")) {
        camera->m_Camera.setFieldOfView(fov);
    }
}

void EditWindowUI::processEditProbeComponent() {
    auto *probe = m_EditorUISystem->getComponent<EnvironmentProbeComponent>(m_selectedEntity);
    if (probe == nullptr) {
        return;
    }

    ImGui::SeparatorText("Environment probe");

    ImGui::ColorEdit3("Debug color##PROBE_COLOR", (float *) &probe->m_DebugColor);
}

bool EditWindowUI::isTransformComponentSelected() {
    if (m_selectedEntity < 0 || m_EditorUISystem->getComponent<TransformComponent>(m_selectedEntity) == nullptr) {
        return false;
    }

    return true;
}

void EditWindowUI::sendEntityCreationEvent(std::string name, std::string entityName) {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::CREATE;
    evt->m_ConfigurationName = std::move(name);
    evt->m_name = std::move(entityName);
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}

void EditWindowUI::sendEntityRemovalEvent() {
    auto evt = new EntityCreationEvent();
    evt->m_Type = EntityCreationEvent::REMOVE;
    evt->m_entityId = m_selectedEntity;
    m_EditorUISystem->m_EventManager->queueEvent(evt);
}
