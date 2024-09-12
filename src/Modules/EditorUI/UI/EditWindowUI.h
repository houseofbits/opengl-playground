#pragma once

#include "ComponentEdit/BaseComponentEdit.h"
#include <string>
#include <map>

class EditorUISystem;

class EditWindowUI {
public:
    explicit EditWindowUI(EditorUISystem *);

    void process();
    bool isTransformComponentSelected();

    int m_selectedEntity;
    bool m_isBoundsTransformAllowed;
private:

    void processEntitiesList();
    void sendEntityCreationEvent(std::string, std::string);
    void sendEntityRemovalEvent();

    EditorUISystem *m_EditorUISystem;
    std::string m_lightProjectorPath;
    std::string m_meshModelPath;
    std::string m_meshMaterialPath;
    int m_selectedEntityCreationType;
    std::map<std::string, BaseComponentEdit*> m_componentEditors;
    std::map<std::string, bool> m_entityListFilter;
    std::string m_filterString;
};
