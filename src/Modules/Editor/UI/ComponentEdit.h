#pragma once

#include "./BaseComponentTransformEdit.h"

template<class ComponentT>
class ComponentEdit : public BaseComponentTransformEdit {
public:
    explicit ComponentEdit(EditorUISystem &editorSystem) : BaseComponentTransformEdit(editorSystem) {
    }

    void handleEntitySelection(Entity &e, Component *c) override {
        m_entity = &e;
        m_component = dynamic_cast<ComponentT *>(c);
    }

protected:
    Entity *m_entity{nullptr};
    ComponentT *m_component{nullptr};
};
