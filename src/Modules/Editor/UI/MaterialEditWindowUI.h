#pragma once
#include "../../../Core/Resources/ResourceHandle.h"
#include "../../Renderer/Resources/MaterialResource.h"

class EditorUISystem;

class MaterialEditWindowUI {
public:
    explicit MaterialEditWindowUI(EditorUISystem *);

    void process();

private:
    void processMaterial();

    void processTexture(const std::string &name, ResourceHandle<TextureResource> &handle) const;

    void saveMaterial() const;

    void createEmptyMaterial() const;

    EditorUISystem *m_EditorUISystem;
    MaterialResource *m_selectedMaterial;
    std::map<std::string, std::string> m_texturePaths;
    std::map<MaterialConfiguration::TextureWrappingType, std::string> m_textureWrappingNames;
    bool m_isNewMaterialSelected;
    std::string m_newMaterialName;
    ResourceHandle<MaterialResource> m_newMaterialResource;
};
