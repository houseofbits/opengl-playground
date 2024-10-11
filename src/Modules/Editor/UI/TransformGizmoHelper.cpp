#include <glm/gtc/type_ptr.hpp>
#include "TransformGizmoHelper.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Physics/Components/PhysicsJointComponent.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../SourceLibs/imgui/imgui_impl_opengl3.h"
#include "../../../SourceLibs/imgui/imgui_impl_sdl2.h"
#include "../../../SourceLibs/imgui/ImGuizmo.h"//Note: Order dependent include. Should be after ImGui

const std::map<long, std::string>TransformGizmoHelper::GizmoOperationNames = {
        {ImGuizmo::TRANSLATE, "Translate"},
        {ImGuizmo::ROTATE,    "Rotate"},
        {ImGuizmo::SCALE,     "Scale"},
};
const std::map<long, std::string>TransformGizmoHelper::GizmoModeNames = {
        {ImGuizmo::WORLD, "WORLD"},
        {ImGuizmo::LOCAL, "LOCAL"}
};

TransformGizmoHelper::TransformGizmoHelper() : m_selectedEntityId(0),
                                               m_selectedTransformOption(TRANSFORM_PRIMARY),
                                               m_transformOptions(),
                                               m_selectedGizmoOperation(ImGuizmo::TRANSLATE),
                                               m_selectedGizmoMode(ImGuizmo::WORLD),
                                               m_transform(1.0) {

}

void TransformGizmoHelper::processGizmo(EntityContext &ctx, Identity::Type &selectedEntityId, Camera &camera) {
    if (selectedEntityId == 0) {
        return;
    }

    if (m_selectedEntityId != selectedEntityId) {
        handleEntitySelection(ctx, selectedEntityId);
    }

//    static float bounds[] = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};
//

    ImGuizmo::BeginFrame();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

    if (ImGuizmo::Manipulate(glm::value_ptr(camera.viewMatrix),
                             glm::value_ptr(camera.projectionMatrix),
                             (ImGuizmo::OPERATION) m_selectedGizmoOperation,
                             (ImGuizmo::MODE) m_selectedGizmoMode,
                             glm::value_ptr(m_transform),
                             nullptr,
                             nullptr,
                             nullptr //m_EditWindowUI.m_isBoundsTransformAllowed ? bounds : nullptr
    )) {
        setSelectedComponentTransform(ctx);
    } else {
        getTransformFromSelectedComponent(ctx);
    }
}

void TransformGizmoHelper::handleEntitySelection(EntityContext &ctx, Identity::Type id) {

    auto entity = ctx.getEntity(id);
    if (entity != nullptr) {
        m_selectedEntityId = id;
        populateTransformOptions(*entity);
        if (m_transformOptions.empty()) {
            m_selectedEntityId = 0;
            return;
        }
        handleOptionSelection(ctx, m_transformOptions.begin()->first);
    }
}

void TransformGizmoHelper::populateTransformOptions(Entity &entity) {
    m_transformOptions.clear();

    auto transformComponent = entity.getComponent<TransformComponent>();
    if (transformComponent != nullptr) {
        TransformOption option;
        option.name = "Primary transform";
        option.isTranslationEnabled = transformComponent->m_isTranslationEnabled;
        option.isRotationEnabled = transformComponent->m_isRotationEnabled;
        option.isScalingEnabled = transformComponent->m_isScalingEnabled;
        if (transformComponent->m_isTranslationEnabled) {
            option.defaultOperation = ImGuizmo::TRANSLATE;
        } else if (transformComponent->m_isRotationEnabled) {
            option.defaultOperation = ImGuizmo::ROTATE;
        } else {
            option.defaultOperation = ImGuizmo::SCALE;
        }
        m_transformOptions[TRANSFORM_PRIMARY] = option;
    }

    if (entity.hasComponent<PhysicsJointComponent>()) {
        TransformOption option;
        option.name = "Hinge joint";
        option.isTranslationEnabled = true;
        option.isRotationEnabled = true;
        option.isScalingEnabled = false;
        option.defaultOperation = ImGuizmo::TRANSLATE;
        m_transformOptions[TRANSFORM_HINGE_JOINT] = option;
    }

    if (entity.hasComponent<CameraComponent>()) {
        TransformOption option;
        option.name = "Camera";
        option.isTranslationEnabled = true;
        option.isRotationEnabled = true;
        option.isScalingEnabled = false;
        option.defaultOperation = ImGuizmo::TRANSLATE;
        m_transformOptions[TRANSFORM_CAMERA] = option;
    }
}

void TransformGizmoHelper::processToolbar(EntityContext &ctx) {
    if (m_transformOptions.empty()) {
        return;
    }

    if (m_transformOptions.size() > 1) {
        ImGui::SameLine();
        ImGui::PushItemWidth(150);
        if (ImGui::BeginCombo("##TRANSFORM_TARGET", m_transformOptions[m_selectedTransformOption].name.c_str())) {
            for (const auto &o: m_transformOptions) {
                if (ImGui::Selectable(o.second.name.c_str(), m_selectedTransformOption == o.first)) {
                    handleOptionSelection(ctx, o.first);
                }
            }
            ImGui::EndCombo();
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##TRANSFORM_TYPE", GizmoOperationNames.at(m_selectedGizmoOperation).c_str())) {
        auto option = m_transformOptions[m_selectedTransformOption];
        if (option.isTranslationEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::TRANSLATE).c_str(),
                                  m_selectedGizmoOperation == ImGuizmo::TRANSLATE)) {
                m_selectedGizmoOperation = ImGuizmo::TRANSLATE;
            }
        }
        if (option.isRotationEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::ROTATE).c_str(),
                                  m_selectedGizmoOperation == ImGuizmo::ROTATE)) {
                m_selectedGizmoOperation = ImGuizmo::ROTATE;
            }
        }
        if (option.isScalingEnabled) {
            if (ImGui::Selectable(GizmoOperationNames.at(ImGuizmo::SCALE).c_str(),
                                  m_selectedGizmoOperation == ImGuizmo::SCALE)) {
                m_selectedGizmoOperation = ImGuizmo::SCALE;
            }
        }
//        //        if (transform->m_isScalingEnabled) {
//        //            if (ImGui::Selectable("Universal", m_currentGizmoOperation == ImGuizmo::SCALEU)) {
//        //                m_currentGizmoOperation = ImGuizmo::SCALEU;
//        //                selected_transform_type_label = "Universal";
//        //            }
//        //        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##TRANSFORM_SPACE", GizmoModeNames.at(m_selectedGizmoMode).c_str())) {
        if (ImGui::Selectable(GizmoModeNames.at(ImGuizmo::WORLD).c_str(),
                              m_selectedGizmoMode == ImGuizmo::WORLD)) {
            m_selectedGizmoMode = ImGuizmo::WORLD;
        }

        if (ImGui::Selectable(GizmoModeNames.at(ImGuizmo::LOCAL).c_str(),
                              m_selectedGizmoMode == ImGuizmo::LOCAL)) {
            m_selectedGizmoMode = ImGuizmo::LOCAL;
        }

        ImGui::EndCombo();
    }

}

void
TransformGizmoHelper::handleOptionSelection(EntityContext &ctx, TransformGizmoHelper::TransformOptionType optionType) {
    m_selectedTransformOption = optionType;
    m_selectedGizmoOperation = m_transformOptions[optionType].defaultOperation;
    m_selectedGizmoMode = ImGuizmo::WORLD;
    getTransformFromSelectedComponent(ctx);
}

void TransformGizmoHelper::setSelectedComponentTransform(EntityContext &ctx) {
    switch (m_selectedTransformOption) {
        case TRANSFORM_PRIMARY:
            setTransformToEditorComponent<TransformComponent>(ctx);
            break;
        case TRANSFORM_HINGE_JOINT:
            setTransformToEditorComponent<PhysicsJointComponent>(ctx);
            break;
        case TRANSFORM_CAMERA:
            setTransformToEditorComponent<CameraComponent>(ctx);
            break;
    }
}

void TransformGizmoHelper::getTransformFromSelectedComponent(EntityContext &ctx) {
    switch (m_selectedTransformOption) {
        case TRANSFORM_PRIMARY:
            getTransformFromEditorComponent<TransformComponent>(ctx);
            break;
        case TRANSFORM_HINGE_JOINT:
            getTransformFromEditorComponent<PhysicsJointComponent>(ctx);
            break;
        case TRANSFORM_CAMERA:
            getTransformFromEditorComponent<CameraComponent>(ctx);
            break;
    }
}
