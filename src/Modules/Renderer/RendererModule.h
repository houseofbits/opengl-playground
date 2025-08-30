#pragma once

#include "../../Core/API.h"
#include "Components/EnvironmentProbeComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Systems/EnvironmentProbeRenderSystem.h"
#include "Systems/MainRenderSystem.h"
#include "Systems/ShadowMapRenderSystem.h"
#include "Systems/StorageBufferUpdateSystem.h"
#include "Systems/OverlayRenderSystem.h"
#include "Systems/PhysicsDebugRenderSystem.h"
#include "Components/MeshComponent.h"
#include "Editors/LightComponentEdit.h"
#include "Editors/StaticMeshComponentEdit.h"
#include "Editors/MeshComponentEdit.h"
#include "Editors/EnvironmentProbeComponentEdit.h"

class RendererModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<StaticMeshComponent>();
        ctx.registerComponent<LightComponent>();
        ctx.registerComponent<EnvironmentProbeComponent>();
        ctx.registerComponent<SkyComponent>();
        ctx.registerComponent<MeshComponent>();
    }

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<StorageBufferUpdateSystem>(EntitySystemRegistry::MAIN_PROCESS, 1);
        ctx.registerEntitySystem<ShadowMapRenderSystem>(EntitySystemRegistry::MAIN_PROCESS, 2);

        ctx.registerEntitySystem<EnvironmentProbeRenderSystem>(EntitySystemRegistry::MAIN_PROCESS, 3);
        ctx.registerEntitySystem<MainRenderSystem>(EntitySystemRegistry::MAIN_PROCESS, 5);
        ctx.registerEntitySystem<PhysicsDebugRenderSystem>(EntitySystemRegistry::MAIN_PROCESS, 6);

        ctx.registerEntitySystem<OverlayRenderSystem>(EntitySystemRegistry::MAIN_PROCESS, 7);
    }

    void registerScriptableTypes(ScriptingManager &scriptingManager) override {
        scriptingManager.registerComponentType<LightComponent>("isEnabled", &LightComponent::m_isEnabled,
                                                               "intensity", &LightComponent::m_Intensity
        );

        scriptingManager.registerComponentType<MeshComponent>(
            "setMaterial", &MeshComponent::setNodeMaterial,
            "getMaterial", &MeshComponent::getNodeMaterial
        );
    }

    void postRegister(EntityContext &ctx) override {
        if (const auto editorSystem = ctx.getSystem<EditorUISystem>(); editorSystem != nullptr) {
            editorSystem->registerComponentEditor<EnvironmentProbeComponent>(processEnvironmentProbeComponentEditor);
            editorSystem->registerComponentEditor<StaticMeshComponent>(processStaticMeshComponentEditor);
            editorSystem->registerComponentEditor<MeshComponent>(processMeshComponentEditor);
            editorSystem->registerComponentEditor<LightComponent>(processLightComponentEditor);
        }
    }
};
