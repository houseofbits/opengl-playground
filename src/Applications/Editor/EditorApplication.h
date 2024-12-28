#pragma once

#include "../../Core/API.h"
#include "../../Modules/Editor/Events/EditorUIEvent.h"
#include "../../Window.h"

class EditorApplication : public Application, public EventListener {
public:
    EditorApplication();

    void run() override;

    void initialize(const std::string &entityDefinitionFileName);

    void handleEditorUIEvent(const EditorUIEvent &);

    //Used for event manager to identify target handlers
    Identity::Type getId() override { return 0; }
};
