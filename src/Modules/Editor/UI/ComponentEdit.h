#pragma once

#include "./BaseComponentEdit.h"

template<class ComponentT>
class ComponentEdit : public BaseComponentEdit {
public:
    explicit ComponentEdit(EditorUISystem &editorSystem) : BaseComponentEdit(editorSystem) {
    }

    void handleEntitySelection(Entity &e, Component *c) override {
        m_entity = &e;
        m_component = dynamic_cast<ComponentT *>(c);
    }

protected:
    Entity *m_entity{nullptr};
    ComponentT *m_component{nullptr};
};
