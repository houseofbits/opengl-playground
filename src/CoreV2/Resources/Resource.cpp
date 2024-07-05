#include "Resource.h"


Resource::Resource() : m_Path(),
                       m_Status(STATUS_UNDEFINED),
                       m_ReferenceCount(0),
                       m_Dependencies() {
}
