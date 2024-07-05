#include "LightsBufferResource.h"

LightsBufferResource::LightsBufferResource() : Resource() {
}

Resource::Status LightsBufferResource::build() {

    return STATUS_READY;
}

void LightsBufferResource::destroy() {
}
