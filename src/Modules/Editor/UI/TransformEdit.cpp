#include "TransformEdit.h"

#include <GL/glew.h>

#include "../Systems/EditorUISystem.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui
#include <glm/gtc/type_ptr.hpp>
#include "../../../Core/Helper/Math.h"
#include "../../../Core/Helper/StringUtils.h"
#include "../../../Renderer/Shader/WireframeRenderer.h"

const std::map<long, std::string> GizmoOperationNames = {
    {ImGuizmo::TRANSLATE, "Translate"},
    {ImGuizmo::ROTATE, "Rotate"},
    {ImGuizmo::SCALE, "Scale"},
};
const std::map<long, std::string> GizmoModeNames = {
    {ImGuizmo::WORLD, "WORLD"},
    {ImGuizmo::LOCAL, "LOCAL"}
};

void TransformEdit::processToolbar() {
    auto *e = m_UISystem->getSelectedEntity();
    if (!e) {
        return;
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(150);

    processTransformComponentDropdown(*e);

    processTransformTargetDropdown(*e);

    if (m_pSelectedComponentEdit != nullptr) {
        ImGui::SameLine();
        ImGui::PushItemWidth(100);

        processTransformOperationDropdown();

        ImGui::SameLine();
        ImGui::PushItemWidth(100);

        processTransformSpaceDropdown();
    }
}

void TransformEdit::processGizmo(Camera &camera) {
    auto *e = m_UISystem->getSelectedEntity();
    if (!e) {
        return;
    }

    if (m_pSelectedComponentEdit == nullptr) {
        return;
    }

    const auto options = m_pSelectedComponentEdit->getTransformTargetOptions(m_selectedTransformTargetIndex);

    static float bounds[] = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};

    ImGuizmo::BeginFrame();
    ImGuizmo::SetOrthographic(camera.isOrthographic);
    ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

    if (ImGuizmo::Manipulate(glm::value_ptr(camera.viewMatrix),
                             glm::value_ptr(camera.projectionMatrix),
                             static_cast<ImGuizmo::OPERATION>(m_selectedTransformOperation),
                             static_cast<ImGuizmo::MODE>(m_selectedTransformSpace),
                             glm::value_ptr(m_transform),
                             nullptr,
                             nullptr,
                             options.isBoundsTransformEnabled ? bounds : nullptr
    )) {
        m_pSelectedComponentEdit->setWorldSpaceTransform(m_transform, m_selectedTransformTargetIndex);
    } else {
        m_transform = m_pSelectedComponentEdit->getWorldSpaceTransform(m_selectedTransformTargetIndex);
    }
}

void TransformEdit::processDebugHelpers(WireframeRenderer &renderer) const {
    if (m_pSelectedComponentEdit == nullptr) {
        return;
    }

    if (m_pSelectedComponentEdit->getNumTransformTargets() == 0) {
        return;
    }

    for (int i = 0; i < m_pSelectedComponentEdit->getNumTransformTargets(); ++i) {
        auto options = m_pSelectedComponentEdit->getTransformTargetOptions(i);
        if (options.isTransformCenterGizmoVisible) {
            glDisable(GL_DEPTH_TEST);

            auto m = Math::rescaleMatrix(m_pSelectedComponentEdit->getWorldSpaceTransform(i), 0.3);
            renderer.renderSphere(m, {0, 0, 1, 1});

            glEnable(GL_DEPTH_TEST);
        }
    }
}

void TransformEdit::handleEntitySelection(Entity &e) {
    m_selectedComponentId = 0;
    m_selectedTransformTargetIndex = -1;
    m_pSelectedComponentEdit = nullptr;
    m_selectedTransformSpace = ImGuizmo::WORLD;
    m_selectedTransformOperation = 0;

    //Try to select TransformComponent first
    if (const auto transformComponent = e.getComponent<TransformComponent>()) {
        if (const auto editor = m_UISystem->m_componentEditors[transformComponent->getTypeName()]) {
            m_selectedComponentId = transformComponent->m_Id.id();
            m_pSelectedComponentEdit = editor;
            m_selectedTransformTargetIndex = 0;
            m_pSelectedComponentEdit->handleEntitySelection(e, transformComponent);
        }
    }

    //If entity does not have transform component the try to find other transformable component
    if (!m_pSelectedComponentEdit) {
        for (const auto &component: e.m_Components) {
            auto editor = m_UISystem->m_componentEditors[component->getTypeName()];
            if (editor && editor->getNumTransformTargets() > 0) {
                m_selectedComponentId = component->m_Id.id();
                m_pSelectedComponentEdit = editor;
                m_selectedTransformTargetIndex = 0;
                m_pSelectedComponentEdit->handleEntitySelection(e, component.get());
                break;
            }
        }
    }

    if (m_pSelectedComponentEdit) {
        if (const auto options = m_pSelectedComponentEdit->getTransformTargetOptions(0);
            options.isTranslationEnabled) {
            m_selectedTransformOperation = ImGuizmo::TRANSLATE;
        } else if (options.isRotationEnabled) {
            m_selectedTransformOperation = ImGuizmo::ROTATE;
        } else if (options.isScalingEnabled) {
            m_selectedTransformOperation = ImGuizmo::SCALE;
        }

        m_transform = m_pSelectedComponentEdit->getWorldSpaceTransform(m_selectedTransformTargetIndex);
    }
}

void TransformEdit::processTransformComponentDropdown(Entity &e) {
    std::string selectedName;
    if (auto c = e.getComponent(m_selectedComponentId); c != nullptr) {
        selectedName = StringUtils::pascalCaseToHumanReadable(c->getTypeName());
    }

    if (ImGui::BeginCombo("##TRANSFORM_COMPONENT", selectedName.c_str())) {
        for (const auto &component: e.m_Components) {
            if (auto editor = m_UISystem->m_componentEditors[component->getTypeName()];
                editor && editor->getNumTransformTargets() > 0) {
                std::string componentTypeName = component->getHumanReadableTypeName();

                if (bool isSelected = m_selectedComponentId == component->m_Id.id();
                    ImGui::Selectable(componentTypeName.c_str(), isSelected)) {
                    m_selectedComponentId = component->m_Id.id();
                    m_pSelectedComponentEdit = editor;
                    editor->handleEntitySelection(e, component.get());
                    m_selectedTransformTargetIndex = 0;
                }
            }
        }

        ImGui::EndCombo();
    }
}

void TransformEdit::processTransformTargetDropdown(Entity &e) {
    if (!m_pSelectedComponentEdit) {
        return;
    }

    if (m_pSelectedComponentEdit->getNumTransformTargets() > 1) {
        const std::string selectedName = m_pSelectedComponentEdit->getTransformTargetOptions(
            m_selectedTransformTargetIndex).name;
        if (ImGui::BeginCombo("##TRANSFORM_TARGET", selectedName.c_str())) {
            for (int i = 0; i < m_pSelectedComponentEdit->getNumTransformTargets(); ++i) {
                if (bool isSelected = i == m_selectedTransformTargetIndex;
                    ImGui::Selectable(m_pSelectedComponentEdit->getTransformTargetOptions(i).name.c_str(),
                                      isSelected)) {
                    m_selectedTransformTargetIndex = i;
                }
            }

            ImGui::EndCombo();
        }
    } else {
        m_selectedTransformTargetIndex = 0;
    }
}

void TransformEdit::processTransformOperationDropdown() {
    if (GizmoOperationNames.count(m_selectedTransformOperation) == 0) {
        return;
    }

    if (ImGui::BeginCombo("##TRANSFORM_OPERATION", GizmoOperationNames.at(m_selectedTransformOperation).c_str())) {
        auto option = m_pSelectedComponentEdit->getTransformTargetOptions(m_selectedTransformTargetIndex);

        if (option.isTranslationEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::TRANSLATE).c_str(),
                                  m_selectedTransformOperation == ImGuizmo::TRANSLATE)) {
                m_selectedTransformOperation = ImGuizmo::TRANSLATE;
            }
        }
        if (option.isRotationEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::ROTATE).c_str(),
                                  m_selectedTransformOperation == ImGuizmo::ROTATE)) {
                m_selectedTransformOperation = ImGuizmo::ROTATE;
            }
        }
        if (option.isScalingEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::SCALE).c_str(),
                                  m_selectedTransformOperation == ImGuizmo::SCALE)) {
                m_selectedTransformOperation = ImGuizmo::SCALE;
            }
        }

        ImGui::EndCombo();
    }
}

void TransformEdit::processTransformSpaceDropdown() {
    if (GizmoModeNames.count(m_selectedTransformSpace) == 0) {
        return;
    }

    if (ImGui::BeginCombo("##TRANSFORM_SPACE", GizmoModeNames.at(m_selectedTransformSpace).c_str())) {
        if (ImGui::Selectable(GizmoModeNames.at(ImGuizmo::WORLD).c_str(),
                              m_selectedTransformSpace == ImGuizmo::WORLD)) {
            m_selectedTransformSpace = ImGuizmo::WORLD;
        }

        if (ImGui::Selectable(GizmoModeNames.at(ImGuizmo::LOCAL).c_str(),
                              m_selectedTransformSpace == ImGuizmo::LOCAL)) {
            m_selectedTransformSpace = ImGuizmo::LOCAL;
        }

        ImGui::EndCombo();
    }
}
