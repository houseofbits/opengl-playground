#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../Components/CameraComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../../Editor/Helpers/TransformHelper.h"

inline void processCameraComponentEditor(Component *c, Entity *e, EditorUISystem &system) {
    auto component = dynamic_cast<CameraComponent *>(c);

    if (component == nullptr) {
        return;
    }

    if (ImGui::Checkbox("Active##CAMERA_ACTIVE", &component->m_isActive)) {
        system.m_EventManager->queueEvent<CameraActivationEvent>(e->m_Id.id());
    }

    ImGui::Checkbox("Is relative rotation disabled##CAMERA_REL_ROTATION",
                    &component->m_isRelativeRotationDisabled);

    float fov = component->m_Camera.fieldOfView;
    if (ImGui::InputFloat("FOV##CAMERA_FOV", &fov, 1.0f, 5.0f, "%.0f")) {
        component->m_Camera.setFieldOfView(fov);
    }

    static int v[2];
    ImGui::Checkbox("Render target##CAMERA_RENDER_TARGET", &component->m_isRenderTargetEnabled);
    if (component->m_isRenderTargetEnabled) {
        if (ImGui::InputInt2("Size", &v[0])) {
            component->m_renderTarget().m_width = v[0];
            component->m_renderTarget().m_height = v[1];
        } else {
            if (component->m_renderTarget.isReady()) {
                v[0] = static_cast<int>(component->m_renderTarget().m_width);
                v[1] = static_cast<int>(component->m_renderTarget().m_height);
            } else {
                v[0] = 64;
                v[1] = 64;
            }
        }
        if (ImGui::Button("Create render target resource")) {
            system.m_ResourceManager->request(component->m_renderTarget, component->getRenderTargetResourceName());
            component->m_renderTarget().resize(v[0], v[1], Texture::Type::TYPE_RGBA);
        }

        ImGui::Text("Name: %s", component->getRenderTargetResourceName().c_str());
    }
}
