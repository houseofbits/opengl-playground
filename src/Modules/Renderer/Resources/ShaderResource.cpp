#include "ShaderResource.h"
#include "../../../Core/Helper/ShaderSourceLoader.h"
#include "RenderShaderResource.h"
#include <vector>
#include "../../../Renderer/Camera/Camera.h"
#include "../../../Renderer/Shader/ShaderBuilder.h"

ShaderResource::ShaderResource() : Resource(), m_shader(), m_uniformResources(), m_namedUniformResources(),
                                   m_isDepthTestEnabled(true), m_shaderBuilder(nullptr) {
}

Resource::Status ShaderResource::fetchData(ResourceManager &manager) {
    auto json = Json::readFile(m_Path);
    if (!json) {
        return STATUS_FETCH_ERROR;
    }

    if (!json->contains("shader")) {
        return STATUS_FETCH_ERROR;
    }

    m_shaderPath = json->at("shader");

    fetchAttributes(*json);
    fetchDependencies(*json, manager);

    if (m_fetchMetadata) {
        m_fetchMetadata = false;

        return STATUS_DATA_FETCHING;
    } else {
        loadShader();

        m_fetchMetadata = true;
    }

    return STATUS_DATA_READY;
}

Resource::Status ShaderResource::build() {
    const auto programId = m_shaderBuilder->compile();
    if (!programId) {
        return STATUS_BUILD_ERROR;
    }

    m_shader.setProgramId(programId.value());

    for (const auto &uniform: m_uniformResources) {
        uniform->get().applyToShader(m_shader);
    }

    for (const auto &[name, uniform]: m_namedUniformResources) {
        uniform->get().applyToShader(m_shader, name);
    }

    return STATUS_READY;
}

void ShaderResource::ShaderResource::destroy() {
}

void ShaderResource::fetchAttributes(nlohmann::json &json) {
    if (json.contains("attributes")) {
        m_isDepthTestEnabled = Json::getValue(json.at("attributes"), "depthTest", true);
    }
}

void ShaderResource::fetchDependencies(nlohmann::json &json, ResourceManager &manager) {
    if (json.contains("uniforms") && json.at("uniforms").is_array()) {
        for (const auto &uniformDef: json.at("uniforms")) {
            if (!uniformDef.contains("resourceName")) {
                continue;
            }

            auto resourceName = uniformDef.at("resourceName");
            auto resourceType = uniformDef.at("resourceType");

            auto resource = new RenderShaderResourceHandle();

            manager.requestAbstract<RenderShaderResourceHandle>(*resource, resourceType, resourceName);

            if (uniformDef.contains("name")) {
                auto name = uniformDef.at("name");
                m_namedUniformResources.emplace_back(name, resource);
            } else {
                m_uniformResources.push_back(resource);
            }
            addDependency(resourceName);
        }
    }
}

void ShaderResource::loadShader() {
    auto paths = StringUtils::getAlternatePaths(m_shaderPath);
    if (paths.empty()) {
        Log::error("Resource path " + m_Path + " not valid for ShaderProgramResource");
        return;
    }

    m_shaderBuilder = new ShaderBuilder();

    for (const auto &path: paths) {
        m_shaderBuilder->loadFromFile(path);
    }
}

void ShaderResource::use(Camera &camera) {
    m_shader.addUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());
    m_shader.addUniform("viewPosition", camera.getPosition());

    for (const auto &uniform: m_uniformResources) {
        uniform->get().applyToShader(m_shader);
    }

    for (const auto &[name, uniform]: m_namedUniformResources) {
        uniform->get().applyToShader(m_shader, name);
    }
}
