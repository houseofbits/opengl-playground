#pragma once

#include "BaseComponentTransformEdit.h"
#include "../../../Core/Entity/Entity.h"
#include <string>
#include <map>

class EditorUISystem;

class EditWindowUI {
public:
    explicit EditWindowUI(EditorUISystem *);

    void process();

    bool isTransformComponentSelected() const;

    bool m_isBoundsTransformAllowed;
    bool m_isEntityCreationWindowOpen;

private:
    void processEntitiesList();

    void sendEntityCreationEvent(std::string, std::string);

    void sendComponentCreationEvent(std::string);

    void sendComponentRemovalEvent(int componentId) const;

    void sendEntityCloneEvent(Identity::Type entityId);

    void sendEntityRemovalEvent();

    void updateEntityNameReferences(Identity::Type, const std::string &);

    void processEntityCreation();

    void processEntityCreationWindow();

    void processComponentEditors(Entity *e) const;

    void processComponentCreation(Entity *e);

    void clearComponentSelection();

    EditorUISystem *m_EditorUISystem;
    std::string m_lightProjectorPath;
    std::string m_meshModelPath;
    std::string m_meshMaterialPath;
    std::string m_selectedEntityCreationType;
    std::string m_selectedComponentCreationType;
    std::string m_entitiesFilterString;
    std::string m_templatesFilterString;
    bool m_isComponentCreationOpen;
    bool m_selectedComponents[30];
};
