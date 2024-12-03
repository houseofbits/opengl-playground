#pragma once

#include "../../../SourceLibs/imgui/imgui.h"
#include "../../../Core/Entity/EntityContext.h"

template<class T>
bool TargetEntityHelper(EntityContext *ctx, std::string &name, const std::string &label) {
    bool select = false;
    if (ImGui::BeginCombo(label.c_str(), name.c_str())) {
        for (const auto &entity: ctx->getAllEntities()) {
            auto *c = entity->getComponent<T>();
            if (c != nullptr) {
                if (ImGui::Selectable(entity->m_Name.c_str(), entity->m_Name == name)) {
                    name = entity->m_Name;
                    select = true;
                }
            }
        }
        ImGui::EndCombo();
    }
    
    return select;
}
