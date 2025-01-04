#pragma once

#include "../../../Core/API.h"
#include "../Resources/WindowResource.h"
#include "../../Editor/Events/EditorUIEvent.h"

class ApplicationSystem : public EntitySystem {
public:
    ApplicationSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleEntityCreationEvent(const EntityCreationEvent &);

    void handleEditorUIEvent(const EditorUIEvent &event);

private:
    void setTime();

    void loadEntitiesFromFile(const std::string &fileName);

    void saveEntitiesToFile() const;

    ResourceManager *m_ResourceManager;
    ResourceHandle<WindowResource> m_WindowResource;

    std::string m_EntitySourceFileName;
};
