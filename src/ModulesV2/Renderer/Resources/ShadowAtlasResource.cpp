#include "ShadowAtlasResource.h"

ShadowAtlasResource::ShadowAtlasResource() : Resource() {
}

Resource::Status ShadowAtlasResource::fetchData(ResourceManager &) {
    return Resource::STATUS_DATA_READY;
}

Resource::Status ShadowAtlasResource::build() {
    return Resource::build();
}

void ShadowAtlasResource::destroy() {
}

void ShadowAtlasResource::bind(ShaderProgramResource &shader) {
}
