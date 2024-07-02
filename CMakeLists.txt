cmake_minimum_required(VERSION 3.16.3)
project(SDLWindow VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB_RECURSE sources src/*.cpp src/*.h)

add_executable(${PROJECT_NAME} ${sources}
        src/Renderer/Renderer/StaticGeometryRenderer.cpp
        src/Renderer/Renderer/StaticGeometryRenderer.h
        src/Renderer/Renderer/BaseRenderer.cpp
        src/Renderer/Renderer/BaseRenderer.h
        src/SceneLoader/SceneLoader.cpp
        src/SceneLoader/SceneLoader.h
        src/Tests/Rendering/TestTangentSpaceCalculation.h
        src/Tests/Rendering/TestNormalMapping.h
        src/Tests/Rendering/TestMipMapping.h
        src/Renderer/Texture/TextureCube.cpp
        src/Renderer/Texture/TextureCube.h
        src/Helper/StringUtils.h
        src/Renderer/Texture/TextureCubeRenderTarget.cpp
        src/Renderer/Texture/TextureCubeRenderTarget.h
        src/Renderer/Renderer/SkyBoxRenderer.cpp
        src/Renderer/Renderer/SkyBoxRenderer.h
        src/Renderer/Common/ProjectionView.cpp
        src/Renderer/Common/ProjectionView.h
        src/Renderer/Camera/CameraController.cpp
        src/Renderer/Camera/CameraController.h
        src/Renderer/Renderer/DebugInformationRenderer.cpp
        src/Renderer/Renderer/DebugInformationRenderer.h
        src/Renderer/EnvironmentProbe/EnvironmentProbe.cpp
        src/Renderer/EnvironmentProbe/EnvironmentProbe.h
        src/Renderer/EnvironmentProbe/EnvironmentProbeUniformBuffer.cpp
        src/Renderer/EnvironmentProbe/EnvironmentProbeUniformBuffer.h
        src/Renderer/Renderer/EnvironmentProbeRenderer.cpp
        src/Renderer/Renderer/EnvironmentProbeRenderer.h
        src/SceneLoader/ModelBuildConfiguration.h
        src/SceneLoader/BaseConfiguration.h
        src/SceneLoader/BaseConfiguration.cpp
        src/SceneLoader/ProbeBuildConfiguration.h
        src/Renderer/Shader/ShaderStorageBuffer.h
        src/Physics/TestPhysics.cpp
        src/Physics/TestPhysics.h
        src/CoreV2/Entity/Entity.h
        src/CoreV2/Reflection/Factory.h
        src/CoreV2/Entity/Component.cpp
        src/CoreV2/Entity/Component.h
        src/CoreV2/Entity/EntitySerializer.h
        src/CoreV2/Entity/EntityContext.cpp
        src/CoreV2/Entity/EntityContext.h
        src/CoreV2/System/EntitySystem.h
        src/CoreV2/Module/EntityModule.cpp
        src/CoreV2/Module/EntityModule.h
        src/ModulesV2/Renderer/RendererModule.h
        src/ModulesV2/Renderer/Components/StaticMeshComponent.cpp
        src/ModulesV2/Renderer/Components/StaticMeshComponent.h
        src/ModulesV2/Common/CommonModule.h
        src/ModulesV2/Common/Components/TransformComponent.cpp
        src/ModulesV2/Common/Components/TransformComponent.h
        src/ModulesV2/Renderer/Systems/RendererSystem.cpp
        src/ModulesV2/Renderer/Systems/RendererSystem.h
        src/CoreV2/API.h
        src/CoreV2/Reflection/Type.h
        src/CoreV2/Entity/Entity.cpp
        src/ModulesV2/Renderer/Components/LightComponent.cpp
        src/ModulesV2/Renderer/Components/LightComponent.h
        src/ModulesV2/Renderer/Components/EnvironmentProbeComponent.cpp
        src/ModulesV2/Renderer/Components/EnvironmentProbeComponent.h
        src/ModulesV2/Renderer/Components/MaterialComponent.cpp
        src/ModulesV2/Renderer/Components/MaterialComponent.h
        src/CoreV2/Application/Application.cpp
        src/CoreV2/Application/Application.h
        src/ModulesV2/Renderer/Components/CameraComponent.cpp
        src/ModulesV2/Renderer/Components/CameraComponent.h
        src/CoreV2/Json/Json.cpp
        src/CoreV2/Json/Json.h
        src/CoreV2/Json/Json.cpp
        src/CoreV2/Json/Json.h
        src/ApplicationsV2/Main/MainApplication.cpp
        src/ApplicationsV2/Main/MainApplication.h
        src/CoreV2/Reflection/Identity.h
        src/CoreV2/Resources/Resource.cpp
        src/CoreV2/Resources/Resource.h
        src/CoreV2/Resources/ResourceManager.cpp
        src/CoreV2/Resources/ResourceManager.h
        src/CoreV2/Resources/ResourceHandle.cpp
        src/CoreV2/Resources/ResourceHandle.h
        src/ResourcesV2/TextureResource.cpp
        src/ResourcesV2/TextureResource.h
        src/ModulesV2/Renderer/Renderers/ColorPassRenderer.cpp
        src/ModulesV2/Renderer/Renderers/ColorPassRenderer.h
        src/ResourcesV2/StaticMeshResource.cpp
        src/ResourcesV2/StaticMeshResource.h
        src/ModulesV2/Renderer/Systems/CameraControlSystem.cpp
        src/ModulesV2/Renderer/Systems/CameraControlSystem.h
        src/ModulesV2/Renderer/Renderers/ColorPassFrameData.h
        src/ModulesV2/Renderer/Buffers/LightStorageBuffer.h
        src/ModulesV2/Renderer/Buffers/LightStorageBuffer.cpp
        src/ResourcesV2/ShaderResource.cpp
        src/ResourcesV2/ShaderResource.h
        src/ResourcesV2/ShaderProgramResource.cpp
        src/ResourcesV2/ShaderProgramResource.h
        src/Helper/StringUtils.cpp
        src/ModulesV2/Renderer/Buffers/ProjectionSamplerStorageBuffer.cpp
        src/ModulesV2/Renderer/Buffers/ProjectionSamplerStorageBuffer.h
        src/ModulesV2/EditorUI/EditorUIModule.h
        src/ModulesV2/EditorUI/Systems/EditorUISystem.cpp
        src/ModulesV2/EditorUI/Systems/EditorUISystem.h
        src/CoreV2/Events/Event.cpp
        src/CoreV2/Events/EntityCreationEvent.h
        src/ModulesV2/EditorUI/UI/MainToolbarUI.cpp
        src/ModulesV2/EditorUI/UI/MainToolbarUI.h
        src/ModulesV2/EditorUI/UI/EditWindowUI.cpp
        src/ModulesV2/EditorUI/UI/EditWindowUI.h
)

target_link_libraries(SDLWindow SDL2main SDL2 GL)

find_package(GLEW REQUIRED)
target_link_libraries(SDLWindow GLEW::GLEW)

find_package(glm REQUIRED)
target_link_libraries(SDLWindow glm::glm)

target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/tinygltf/build/libtinygltf.a)

target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/PhysX/bin/linux.clang/release/libPhysX_static_64.a)

target_compile_features(SDLWindow PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
include(CPack)
