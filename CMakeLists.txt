cmake_minimum_required(VERSION 3.16.3)
project(SDLWindow VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB_RECURSE sources src/*.cpp src/*.h)

add_executable(${PROJECT_NAME} ${sources}
        src/Renderer/Texture/TextureCube.cpp
        src/Renderer/Texture/TextureCube.h
        src/Core/Helper/StringUtils.h
        src/Renderer/Texture/TextureCubeRenderTarget.cpp
        src/Renderer/Texture/TextureCubeRenderTarget.h
        src/Renderer/Shader/ShaderStorageBuffer.h
        src/Core/Entity/Entity.h
        src/Core/Reflection/Factory.h
        src/Core/Entity/Component.cpp
        src/Core/Entity/Component.h
        src/Core/Entity/EntitySerializer.h
        src/Core/Entity/EntityContext.cpp
        src/Core/Entity/EntityContext.h
        src/Core/System/EntitySystem.h
        src/Core/Module/EntityModule.cpp
        src/Core/Module/EntityModule.h
        src/Modules/Renderer/RendererModule.h
        src/Modules/Renderer/Components/StaticMeshComponent.cpp
        src/Modules/Renderer/Components/StaticMeshComponent.h
        src/Modules/Common/CommonModule.h
        src/Modules/Common/Components/TransformComponent.cpp
        src/Modules/Common/Components/TransformComponent.h
        src/Core/API.h
        src/Core/Reflection/Type.h
        src/Core/Entity/Entity.cpp
        src/Modules/Renderer/Components/LightComponent.cpp
        src/Modules/Renderer/Components/LightComponent.h
        src/Modules/Renderer/Components/EnvironmentProbeComponent.cpp
        src/Modules/Renderer/Components/EnvironmentProbeComponent.h
        src/Core/Application/Application.cpp
        src/Core/Application/Application.h
        src/Modules/Common/Components/CameraComponent.cpp
        src/Modules/Common/Components/CameraComponent.h
        src/Core/Json/Json.cpp
        src/Core/Json/Json.h
        src/Core/Json/Json.cpp
        src/Core/Json/Json.h
        src/Applications/Main/MainApplication.cpp
        src/Applications/Main/MainApplication.h
        src/Core/Reflection/Identity.h
        src/Core/Resources/Resource.cpp
        src/Core/Resources/Resource.h
        src/Core/Resources/ResourceManager.cpp
        src/Core/Resources/ResourceManager.h
        src/Core/Resources/ResourceHandle.cpp
        src/Core/Resources/ResourceHandle.h
        src/Modules/Renderer/Resources/TextureResource.cpp
        src/Modules/Renderer/Resources/TextureResource.h
        src/Modules/Renderer/Resources/StaticMeshResource.cpp
        src/Modules/Renderer/Resources/StaticMeshResource.h
        src/Modules/Common/Systems/CameraControlSystem.cpp
        src/Modules/Common/Systems/CameraControlSystem.h
        src/Modules/Renderer/Buffers/LightStorageBuffer.h
        src/Modules/Renderer/Buffers/LightStorageBuffer.cpp
        src/Modules/Renderer/Resources/ShaderResource.cpp
        src/Modules/Renderer/Resources/ShaderResource.h
        src/Modules/Renderer/Resources/ShaderProgramResource.cpp
        src/Modules/Renderer/Resources/ShaderProgramResource.h
        src/Core/Helper/StringUtils.cpp
        src/Modules/EditorUI/EditorUIModule.h
        src/Modules/EditorUI/Systems/EditorUISystem.cpp
        src/Modules/EditorUI/Systems/EditorUISystem.h
        src/Core/Events/Event.cpp
        src/Core/Events/EntityCreationEvent.h
        src/Modules/EditorUI/UI/MainToolbarUI.cpp
        src/Modules/EditorUI/UI/MainToolbarUI.h
        src/Modules/EditorUI/UI/EditWindowUI.cpp
        src/Modules/EditorUI/UI/EditWindowUI.h
        src/Modules/Renderer/Resources/MaterialResource.cpp
        src/Modules/Renderer/Resources/MaterialResource.h
        src/Modules/Renderer/Buffers/EnvironmentProbeStorageBuffer.cpp
        src/Modules/Renderer/Buffers/EnvironmentProbeStorageBuffer.h
        src/Modules/Renderer/Systems/MainRenderSystem.cpp
        src/Modules/Renderer/Systems/MainRenderSystem.h
        src/Modules/Renderer/Resources/LightsBufferResource.cpp
        src/Modules/Renderer/Resources/LightsBufferResource.h
        src/Modules/Renderer/Systems/StorageBufferUpdateSystem.cpp
        src/Modules/Renderer/Systems/StorageBufferUpdateSystem.h
        src/Modules/Renderer/Resources/EnvironmentProbesCubeMapArrayResource.cpp
        src/Modules/Renderer/Resources/EnvironmentProbesCubeMapArrayResource.h
        src/Modules/Renderer/Systems/ShadowMapRenderSystem.cpp
        src/Modules/Renderer/Systems/ShadowMapRenderSystem.h
        src/Modules/Renderer/Resources/ShadowMapResource.cpp
        src/Modules/Renderer/Resources/ShadowMapResource.h
        src/Modules/EditorUI/UI/FileDialogHelper.h
        src/Modules/EditorUI/UI/MaterialEditWindowUI.cpp
        src/Modules/EditorUI/UI/MaterialEditWindowUI.h
        src/Modules/EditorUI/UI/TexturePreviewHelper.h
        src/Modules/EditorUI/UI/TextPromptHelper.h
        src/Modules/Physics/Systems/PhysicsSystem.cpp
        src/Modules/Physics/Systems/PhysicsSystem.h
        src/Modules/Physics/Components/RigidBodyComponent.cpp
        src/Modules/Physics/Components/RigidBodyComponent.h
        src/Modules/EditorUI/UI/ComponentEdit/BaseComponentEdit.h
        src/Modules/EditorUI/UI/ComponentEdit/TransformComponentEdit.h
        src/Modules/Physics/Components/CharacterControllerComponent.cpp
        src/Modules/Physics/Components/CharacterControllerComponent.h
        src/Core/Helper/Types.h
        src/Modules/Physics/Components/PhysicsMeshComponent.cpp
        src/Modules/Physics/Components/PhysicsMeshComponent.h
        src/Modules/Physics/Resources/PhysicsMeshResource.cpp
        src/Modules/Physics/Resources/PhysicsMeshResource.h
        src/Modules/Physics/Resources/PhysicsResource.cpp
        src/Modules/Physics/Resources/PhysicsResource.h
        src/Modules/Physics/Systems/PhysicsBodyProcessingSystem.cpp
        src/Modules/Physics/Systems/PhysicsBodyProcessingSystem.h
        src/Modules/Physics/Systems/CharacterControllerSystem.cpp
        src/Modules/Physics/Systems/CharacterControllerSystem.h
)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries (SDLWindow ${SDL2_LIBRARIES})

target_link_libraries(SDLWindow GL)

find_package(GLEW REQUIRED)
target_link_libraries(SDLWindow GLEW::GLEW)

find_package(glm REQUIRED)
target_link_libraries(SDLWindow glm::glm)

target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/tinygltf/build/libtinygltf.a)

include_directories(${CMAKE_SOURCE_DIR}/libs/PhysX/include)
target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/PhysX/bin/linux.clang/release/libPhysX_static_64.a)
target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/PhysX/bin/linux.clang/release/libPhysXCooking_static_64.a)
target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/PhysX/bin/linux.clang/release/libPhysXCharacterKinematic_static_64.a)
target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/PhysX/bin/linux.clang/release/libPhysXCommon_static_64.a)
target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/PhysX/bin/linux.clang/release/libPhysXExtensions_static_64.a)
target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/PhysX/bin/linux.clang/release/libPhysXFoundation_static_64.a)

target_compile_features(SDLWindow PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
include(CPack)
