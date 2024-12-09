#include "StaticMeshComponent.h"

StaticMeshComponent::StaticMeshComponent() : Component(), m_Mesh(), m_Material(), m_targetRenderer(SOLID) {
    m_TargetRenderNameMap = {
        {SOLID, "SOLID"},
        {SHADOW, "SHADOW_ONLY"}
    };
}
