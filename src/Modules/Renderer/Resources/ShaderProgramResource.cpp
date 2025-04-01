#include "ShaderProgramResource.h"
#include "../../../Core/Helper/StringUtils.h"
#include <vector>

ShaderProgramResource::ShaderProgramResource() : Resource(), Shader() {
}

Resource::Status ShaderProgramResource::fetchData(ResourceManager& manager) {
    auto paths = StringUtils::getAlternatePaths(m_Path);
    if (paths.empty()) {
        Log::error("Resource path " + m_Path + " not valid for ShaderProgramResource");
        return STATUS_FETCH_ERROR;
    }

    for (const auto &path: paths) {
        loadFromFile(path);
    }

    return STATUS_DATA_READY;
}

Resource::Status ShaderProgramResource::build() {
    compileAndLink();

    return STATUS_READY;
}

void ShaderProgramResource::destroy() {

}