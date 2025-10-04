#include "ShaderResource.h"
#include "../../../Core/Helper/Console.h"
#include "../../../Core/Helper/ShaderSourceLoader.h"
#include <GL/glew.h>
#include <vector>

ShaderResource::ShaderResource() : Resource(), m_isDepthTestEnabled(true) {
}

Resource::Status ShaderResource::fetchData(ResourceManager &) {
    auto json = Json::readFile(m_Path);
    if (!json) {
        return STATUS_FETCH_ERROR;
    }

    if (!json->contains("shader")) {
        return STATUS_FETCH_ERROR;
    }

    auto paths = StringUtils::getAlternatePaths(json->at("shader"));
    if (paths.empty()) {
        Log::error("Resource path " + m_Path + " not valid for ShaderProgramResource");
        return STATUS_FETCH_ERROR;
    }

    for (const auto &path: paths) {
        m_shader.loadFromFile(path);
    }

    if (json->contains("attributes")) {
        m_isDepthTestEnabled = Json::getValue(json->at("attributes"), "depthTest", true);
    }

    if (json->contains("uniforms") && json->at("uniforms").is_array()) {
        for (const auto uniformDef: json->at("uniforms")) {
            if (!uniformDef.contains("resourceName")) {
                continue;
            }

            auto resourceName = uniformDef.at("resourceName");

            addDependency(resourceName);
        }
    }

    return STATUS_DATA_READY;
}

Resource::Status ShaderResource::build() {
    m_shader.compileAndLink();

    return STATUS_READY;
}

void ShaderResource::destroy() {
}
