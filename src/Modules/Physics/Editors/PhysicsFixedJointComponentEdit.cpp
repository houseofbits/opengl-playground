#include "./PhysicsFixedJointComponentEdit.h"
#include "../../Common/Editors/EntityLinkedComponentEdit.h"
#include "../../Editor/Systems/EditorUISystem.h"

void PhysicsFixedJointComponentEdit::processEditor() {
    if (m_component == nullptr) {
        return;
    }

    EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *m_editorSystem->m_EntityContext,
        m_component->m_targetEntityAName,
        "Attachment entity A##TRANSFORM_PARENT_ENTITY_NAME_A",
        "Self"
    );

    EntityLinkedComponentEdit::processBasic<PhysicsBodyComponent>(
        *m_editorSystem->m_EntityContext,
        m_component->m_targetEntityBName,
        "Attachment entity B##TRANSFORM_PARENT_ENTITY_NAME_B"
    );
}
