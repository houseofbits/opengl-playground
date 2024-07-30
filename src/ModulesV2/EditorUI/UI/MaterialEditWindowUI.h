#pragma once
#include "../../../CoreV2/Resources/ResourceHandle.h"
#include "../../Renderer/Resources/MaterialResource.h"

class EditorUISystem;

class MaterialEditWindowUI {
public:
    explicit MaterialEditWindowUI(EditorUISystem *);

    void process();
    void open(ResourceHandle<MaterialResource>& handle);
    void processTexture(std::string name, ResourceHandle<TextureResource>& handle) const;

    EditorUISystem *m_EditorUISystem;

    ResourceHandle<MaterialResource> m_material;
};
