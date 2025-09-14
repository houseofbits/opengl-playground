cmake_minimum_required(VERSION 3.16.3)
project(SDLWindow VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE sources src/*.cpp src/*.h)

add_compile_options(-g -gdwarf-4)

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
        src/Applications/Editor/EditorApplication.cpp
        src/Applications/Editor/EditorApplication.h
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
        src/Modules/Renderer/Buffers/LightStorageBuffer.h
        src/Modules/Renderer/Buffers/LightStorageBuffer.cpp
        src/Modules/Renderer/Resources/ShaderResource.cpp
        src/Modules/Renderer/Resources/ShaderResource.h
        src/Modules/Renderer/Resources/ShaderProgramResource.cpp
        src/Modules/Renderer/Resources/ShaderProgramResource.h
        src/Core/Helper/StringUtils.cpp
        src/Modules/Editor/EditorUIModule.h
        src/Modules/Editor/Systems/EditorUISystem.cpp
        src/Modules/Editor/Systems/EditorUISystem.h
        src/Core/Events/Event.cpp
        src/Core/Events/EntityCreationEvent.h
        src/Modules/Editor/UI/MainToolbarUI.cpp
        src/Modules/Editor/UI/MainToolbarUI.h
        src/Modules/Editor/UI/EditWindowUI.cpp
        src/Modules/Editor/UI/EditWindowUI.h
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
        src/Modules/Editor/Helpers/FileDialogHelper.h
        src/Modules/Editor/UI/MaterialEditWindowUI.cpp
        src/Modules/Editor/UI/MaterialEditWindowUI.h
        src/Modules/Editor/Helpers/TexturePreviewHelper.h
        src/Modules/Editor/Helpers/TextPromptHelper.h
        src/Modules/Physics/Systems/PhysicsSystem.cpp
        src/Modules/Physics/Systems/PhysicsSystem.h
        src/Modules/Physics/Components/PhysicsCharacterComponent.cpp
        src/Modules/Physics/Components/PhysicsCharacterComponent.h
        src/Core/Helper/Types.h
        src/Modules/Physics/Resources/PhysicsMeshResource.cpp
        src/Modules/Physics/Resources/PhysicsMeshResource.h
        src/Modules/Physics/Resources/PhysicsResource.cpp
        src/Modules/Physics/Resources/PhysicsResource.h
        src/Modules/Physics/Systems/PhysicsBodyProcessingSystem.cpp
        src/Modules/Physics/Systems/PhysicsBodyProcessingSystem.h
        src/Modules/Physics/Systems/CharacterControllerSystem.cpp
        src/Modules/Physics/Systems/CharacterControllerSystem.h
        src/Modules/Physics/Helpers/PhysicsRayCastResult.h
        src/Modules/Physics/Events/PhysicsPickingEvent.h
        src/Modules/Physics/Components/PhysicsBodyComponent.cpp
        src/Modules/Physics/Components/PhysicsBodyComponent.h
        src/Modules/Physics/Components/PhysicsHingeJointComponent.cpp
        src/Modules/Physics/Components/PhysicsHingeJointComponent.h
        src/Modules/Physics/Systems/JointsProcessingSystem.cpp
        src/Modules/Physics/Systems/JointsProcessingSystem.h
        src/Modules/Physics/Helpers/PhysicsTypeCast.h
        src/Modules/Physics/Helpers/ObjectLayerPairFilterImpl.h
        src/Modules/Physics/Helpers/Layers.h
        src/Modules/Physics/Helpers/BroadPhaseLayerMapper.h
        src/Modules/Physics/Helpers/ObjectVsBroadPhaseLayerFilterImpl.h
        src/Modules/Physics/Helpers/ContactListenerImpl.h
        src/Modules/Renderer/Helpers/PhysicsDebugRenderer.cpp
        src/Modules/Renderer/Helpers/PhysicsDebugRenderer.h
        src/Modules/Renderer/Systems/OverlayRenderSystem.cpp
        src/Modules/Renderer/Systems/OverlayRenderSystem.h
        src/Modules/Common/Systems/TransformHierarchyProcessingSystem.cpp
        src/Modules/Common/Systems/TransformHierarchyProcessingSystem.h
        src/Modules/Physics/Components/PhysicsFixedJointComponent.cpp
        src/Modules/Physics/Components/PhysicsFixedJointComponent.h
        src/Modules/Physics/Helpers/BasePhysicsJoint.h
        src/Modules/Editor/Helpers/TargetEntityHelper.h
        src/Modules/Renderer/Systems/PhysicsDebugRenderSystem.cpp
        src/Modules/Renderer/Systems/PhysicsDebugRenderSystem.h
        src/Modules/Editor/Components/EditorCameraComponent.cpp
        src/Modules/Editor/Components/EditorCameraComponent.h
        src/Modules/Editor/Systems/EditorCameraSystem.cpp
        src/Modules/Editor/Systems/EditorCameraSystem.h
        src/Modules/Common/Events/CameraActivationEvent.h
        src/Modules/Common/Systems/CameraSystem.cpp
        src/Modules/Common/Systems/CameraSystem.h
        src/Modules/Common/Components/BaseCameraComponent.h
        src/Modules/Common/Helpers/ActiveCameraHelper.h
        src/Modules/Behaviours/Components/MainCharacterBehaviourComponent.cpp
        src/Modules/Behaviours/Components/MainCharacterBehaviourComponent.h
        src/Modules/Behaviours/Systems/MainCharacterBehaviourSystem.cpp
        src/Modules/Behaviours/Systems/MainCharacterBehaviourSystem.h
        src/Modules/Behaviours/Components/DoorBehaviourComponent.cpp
        src/Modules/Behaviours/Components/DoorBehaviourComponent.h
        src/Modules/Behaviours/Systems/DoorBehaviourSystem.cpp
        src/Modules/Behaviours/Systems/DoorBehaviourSystem.h
        src/Core/System/ComponentsTuple.cpp
        src/Core/System/ComponentsTuple.h
        src/Core/System/AbstractComponentRegistry.h
        src/Modules/Common/Helpers/EntityLinkedComponent.h
        src/Modules/Common/Systems/EntityLinkingSystem.cpp
        src/Modules/Common/Systems/EntityLinkingSystem.h
        src/Modules/Common/Editors/EntityLinkedComponentEdit.h
        src/Modules/Physics/Helpers/ContactListenerImpl.cpp
        src/Modules/Physics/Helpers/SensorLayerFilter.h
        src/Core/System/EntitySystemRegistry.h
        src/Core/System/BaseSystemProcess.h
        src/Core/System/ContinuousSystemProcess.cpp
        src/Core/System/ContinuousSystemProcess.h
        src/Modules/Application/ApplicationModule.h
        src/Modules/Application/Resources/WindowResource.cpp
        src/Modules/Application/Resources/WindowResource.h
        src/Modules/Physics/Helpers/Builder/PhysicsBuilder.cpp
        src/Modules/Physics/Helpers/Builder/PhysicsBuilder.h
        src/Modules/Renderer/Resources/MeshResource.cpp
        src/Modules/Renderer/Resources/MeshResource.h
        src/Modules/Application/Events/EntityPersistenceEvent.h
        src/Modules/Physics/Helpers/BasePhysicsJoint.cpp
        src/Modules/Physics/Helpers/PhysicsComponent.h
        src/Core/System/EntitySystem.cpp
        src/Core/System/EntitySystemRegistry.cpp
        src/Core/System/BaseSystemProcess.cpp
        src/Renderer/Model/WireModel.cpp
        src/Renderer/Model/WireModel.h
        src/Renderer/Model/WireCube.cpp
        src/Renderer/Model/WireCube.h
        src/Renderer/Model/WireSphere.cpp
        src/Renderer/Model/WireSphere.h
        src/Core/Helper/Math.h
        src/Modules/Editor/Helpers/TransformHelper.h
        src/Modules/Editor/Helpers/TransformHelper.h
        src/Modules/Common/Editors/TransformComponentEdit.h
        src/Modules/Common/Editors/CameraComponentEdit.h
        src/Modules/Physics/Editors/PhysicsSliderJointComponentTransformEdit.cpp
        src/Modules/Physics/Editors/PhysicsSliderJointComponentTransformEdit.h
        src/Renderer/Material/MaterialConfiguration.h
        src/Renderer/Model/ModelGLTFLoader.cpp
        src/Renderer/Model/ModelGLTFLoader.h
        src/Renderer/Model/GLTFFileLoader.cpp
        src/Renderer/Model/GLTFFileLoader.h
        src/Core/Helper/File.h
        src/Renderer/Model/RenderModelBuilder.cpp
        src/Renderer/Model/RenderModelBuilder.h
        src/Core/Helper/DoubleBuffer.h
        src/Core/Scripting/ScriptingManager.cpp
        src/Core/Scripting/ScriptingManager.h
        src/Core/Scripting/ScriptingEventCallback.h
        src/Modules/Physics/Helpers/CompoundShape/PhysicsCompoundShapeBuilder.cpp
        src/Modules/Physics/Helpers/CompoundShape/PhysicsCompoundShapeBuilder.h
        src/Modules/Physics/Editors/PhysicsShapeComponentTransformEdit.cpp
        src/Modules/Physics/Editors/PhysicsShapeComponentTransformEdit.h
        src/Modules/Physics/Helpers/Behaviours/StatefulJointBehaviour.cpp
        src/Modules/Physics/Helpers/Behaviours/StatefulJointBehaviour.h
        src/Modules/Physics/Editors/StatefulJointBehaviourEdit.h
        src/Modules/Renderer/Resources/DeferredRenderTargetResource.cpp
        src/Modules/Renderer/Resources/DeferredRenderTargetResource.h
        src/Modules/Physics/Helpers/Behaviours/LookAtJointBehaviour.cpp
        src/Modules/Physics/Helpers/Behaviours/LookAtJointBehaviour.h
        src/Modules/Renderer/Resources/MaterialsBufferResource.cpp
        src/Modules/Renderer/Resources/MaterialsBufferResource.h
        src/Modules/Physics/PhysicsModule.cpp
)

target_compile_features(SDLWindow PRIVATE cxx_std_17)

IF (WIN32)
    set_source_files_properties(src/Modules/Physics/PhysicsModule.cpp PROPERTIES COMPILE_FLAGS "-g0")

    set(CMAKE_EXE_LINKER_FLAGS "-static")

    target_link_libraries(SDLWindow OPENGL32.LIB)

    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/libs/SDL2/cmake)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/libs/glew)
    include_directories(${CMAKE_SOURCE_DIR}/libs/glm)

    find_package(SDL2 REQUIRED)
    include_directories(${CMAKE_SOURCE_DIR}/libs/SDL2/x86_64-w64-mingw32/include)
    target_link_libraries(SDLWindow ${SDL2_LIBRARIES})

    include_directories(${CMAKE_SOURCE_DIR}/libs/glew/include)
    target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/glew/lib/Debug/x64/glew32d.lib)
#    find_package(GLEW REQUIRED)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    target_link_libraries(SDLWindow ${GLEW_LIBRARIES})

    add_compile_definitions(JPH_PROFILE_ENABLED)
    add_compile_definitions(JPH_DEBUG_RENDERER)
    add_compile_definitions(JPH_OBJECT_STREAM)

    include_directories(${CMAKE_SOURCE_DIR}/libs/Jolt)
    target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/Jolt/libJolt.a)

    target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/tinygltf/libtinygltf.a)

    include_directories(${CMAKE_SOURCE_DIR}/libs/lua/include)
    include_directories(${CMAKE_SOURCE_DIR}/libs/sol2/include)

    target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/sol2/liblua-5.4.4.dll.a)

ELSE ()
    #    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -frtti")
    #    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -frtti")

    target_link_libraries(SDLWindow GL)

    find_package(glm REQUIRED)
    target_link_libraries(SDLWindow glm::glm)

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(SDLWindow ${SDL2_LIBRARIES})

    find_package(GLEW REQUIRED)
    target_link_libraries(SDLWindow GLEW::GLEW)

    add_compile_definitions(JPH_PROFILE_ENABLED)
    add_compile_definitions(JPH_DEBUG_RENDERER)
    add_compile_definitions(JPH_OBJECT_STREAM)

    include_directories(${CMAKE_SOURCE_DIR}/libs/Jolt)
    target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/Jolt/libJolt.a)

    target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/tinygltf/cmake-build-release/libtinygltf.a)

    include_directories(${CMAKE_SOURCE_DIR}/libs/lua/lua)
    include_directories(${CMAKE_SOURCE_DIR}/libs/sol2/include)

    target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/sol2/liblua-5.4.4.so)

    #    include_directories(${CMAKE_SOURCE_DIR}/libs/fbxsdk/Linux/2019.2/include)
#    target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/fbxsdk/Linux/2019.2/lib/debug/libfbxsdk.so)
ENDIF ()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)
include(CPack)
