#pragma once
#include <string>
#include "../../../SourceLibs/imgui/imgui.h"
#include "../Helpers/EntityLinkedComponent.h"
#include "../Events/EntityLinkingEvent.h"

class EntityLinkingEvent;
class EntityLinkedComponent;
class EntityContext;

class EntityLinkedComponentEdit {
public:
    EntityLinkedComponentEdit();

    static void process(EventManager &evt, EntityContext &ctx, Component *comp, const std::string &name) {
        auto linkingComp = comp->getInstance<EntityLinkedComponent>();
        if (!linkingComp) {
            return;
        }

        if (ImGui::BeginCombo(name.c_str(), linkingComp->getLinkedEntityName().c_str())) {
            for (const auto &e: ctx.getAllEntities()) {
                if (ImGui::Selectable(e->m_Name.c_str(), e->m_Id.id() == linkingComp->getLinkedEntityId())) {
                    evt.queueEvent<EntityLinkingEvent>(comp->m_Id.id(), e->m_Id.id());
                }
            }

            if (ImGui::Selectable(" - ", linkingComp->isLinkableToEntity())) {
                evt.queueEvent<EntityLinkingEvent>(comp->m_Id.id());
            }
            ImGui::EndCombo();
        }
    }

    template<class T>
    static void process(EventManager &evt, EntityContext &ctx, Component *comp, const std::string &name) {
        auto linkingComp = comp->getInstance<EntityLinkedComponent>();
        if (!linkingComp) {
            return;
        }

        if (ImGui::BeginCombo(name.c_str(), linkingComp->getLinkedEntityName().c_str())) {
            for (const auto &e: ctx.getAllEntities()) {
                if (e->hasComponent<T>() && ImGui::Selectable(e->m_Name.c_str(),
                                                              e->m_Id.id() == linkingComp->getLinkedEntityId())) {
                    evt.queueEvent<EntityLinkingEvent>(comp->m_Id.id(), e->m_Id.id());
                }
            }

            if (ImGui::Selectable(" - ", linkingComp->isLinkableToEntity())) {
                evt.queueEvent<EntityLinkingEvent>(comp->m_Id.id());
            }
            ImGui::EndCombo();
        }
    }
};
