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

    bool m_isBoundsTransformAllowed;
    bool m_isEntityCreationWindowOpen;
private:

    void processEntitiesList();
    void sendEntityCreationEvent(std::string, std::string);
    void sendComponentCreationEvent(std::string);
    void sendComponentRemovalEvent(std::string);
    void sendEntityCloneEvent(Identity::Type entityId);
    void sendEntityRemovalEvent();
    void updateEntityNameReferences(Identity::Type, const std::string&);

    void processEntityCreation();
    void processEntityCreationWindow();
    void processComponentCreation();

    EditorUISystem *m_EditorUISystem;
    std::string m_lightProjectorPath;
    std::string m_meshModelPath;
    std::string m_meshMaterialPath;
    std::string m_selectedEntityCreationType;
    std::string m_selectedComponentCreationType;
    std::string m_entitiesFilterString;
    std::string m_templatesFilterString;
};
