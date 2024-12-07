#pragma once

#include "BaseComponentEdit.h"
#include "../../../Core/Entity/Entity.h"
#include <string>
#include <map>

class EditorUISystem;

class EditWindowUI {
public:
    explicit EditWindowUI(EditorUISystem *);

    void process();
    bool isTransformComponentSelected();

    Identity::Type m_selectedEntityId;
    bool m_isBoundsTransformAllowed;
private:

    void processEntitiesList();
    void sendEntityCreationEvent(std::string, std::string);
    void sendComponentCreationEvent(std::string);
    void sendComponentRemovalEvent(std::string);
    void sendEntityCloneEvent(Identity::Type entityId);
    void sendEntityRemovalEvent();
    void updateEntityNameReferences(Identity::Type, const std::string&);

    void processEntityCreation();
    void processComponentCreation();

    EditorUISystem *m_EditorUISystem;
    std::string m_lightProjectorPath;
    std::string m_meshModelPath;
    std::string m_meshMaterialPath;
    std::string m_selectedEntityCreationType;
    std::string m_selectedComponentCreationType;
    std::string m_filterString;
};
