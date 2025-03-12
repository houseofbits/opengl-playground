#pragma once
#include "../../../Core/Resources/ResourceHandle.h"
#include "../../Renderer/Resources/MaterialResource.h"

class EditorUISystem;

class MaterialEditWindowUI {
public:
    explicit MaterialEditWindowUI(EditorUISystem *);

    void process();
    void open(ResourceHandle<MaterialResource>& handle);
    void processTexture(const std::string& name, ResourceHandle<TextureResource>& handle) const;

    EditorUISystem *m_EditorUISystem;
    ResourceHandle<MaterialResource> m_material;
    std::map<std::string, std::string> m_texturePaths;
    std::map<MaterialConfiguration::TextureWrappingType, std::string> m_textureWrappingNames;
};
