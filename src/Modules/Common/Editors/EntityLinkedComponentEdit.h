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

    static bool processBasic(EntityContext &ctx, std::string &value, const std::string &name, const std::string &defaultValue = " - ") {
        bool hasChanged = false;
        if (ImGui::BeginCombo(name.c_str(), value.empty() ? defaultValue.c_str() : value.c_str())) {
            for (const auto &e: ctx.getAllEntities()) {
                if (ImGui::Selectable(e->m_Name.c_str(), e->m_Name == value)) {
                    hasChanged = true;
                    value = e->m_Name;
                }
            }

            if (ImGui::Selectable(defaultValue.c_str(), value.empty())) {
                hasChanged = true;
                value = "";
            }
            ImGui::EndCombo();
        }

        return hasChanged;
    }

    template<typename... Rest>
    static bool processMulti(EntityContext &ctx, std::string &value, const std::string &name, const std::string &defaultValue = " - ") {
        bool hasChanged = false;
        if (ImGui::BeginCombo(name.c_str(), value.empty() ? defaultValue.c_str() : value.c_str())) {
            for (const auto &e: ctx.getAllEntities()) {
                // if (e->hasComponent<T>() && ImGui::Selectable(e->m_Name.c_str(), e->m_Name == value)) {
                //     hasChanged = true;
                //     value = e->m_Name;
                // }
                hasChanged = selectableType<Rest...>(e.get(), value);
            }

            if (ImGui::Selectable(defaultValue.c_str(), value.empty())) {
                hasChanged = true;
                value = "";
            }
            ImGui::EndCombo();
        }

        return hasChanged;
    }

    template<typename Last>
    static bool selectableType(Entity* e, std::string &value) {
        bool hasChanged = false;

        if (e->hasComponent<Last>() && ImGui::Selectable(e->m_Name.c_str(), e->m_Name == value)) {
            hasChanged = true;
            value = e->m_Name;
        }

        return hasChanged;
    }

    template<typename First, typename Second, typename... Rest>
    static bool selectableType(Entity* e, std::string &value) {
        const bool hasChanged = selectableType<First>(e, value);
        const bool hasChangedRest = selectableType<Second, Rest...>(e, value);

        return hasChanged || hasChangedRest;
    }

    template<class T>
    static bool processBasic(EntityContext &ctx, std::string &value, const std::string &name, const std::string &defaultValue = " - ") {
        bool hasChanged = false;
        if (ImGui::BeginCombo(name.c_str(), value.empty() ? defaultValue.c_str() : value.c_str())) {
            for (const auto &e: ctx.getAllEntities()) {
                if (e->hasComponent<T>() && ImGui::Selectable(e->m_Name.c_str(), e->m_Name == value)) {
                    hasChanged = true;
                    value = e->m_Name;
                }
            }

            if (ImGui::Selectable(defaultValue.c_str(), value.empty())) {
                hasChanged = true;
                value = "";
            }
            ImGui::EndCombo();
        }

        return hasChanged;
    }

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
