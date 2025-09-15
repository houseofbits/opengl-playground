# Common

* **JoltPhysics** https://github.com/jrouwe/JoltPhysics

  Debug build with `CPP_RTTI_ENABLED`
  ```CMake
  option(CPP_RTTI_ENABLED "Enable C++ RTTI" ON)
  ```

* **tinygltf** https://github.com/syoyo/tinygltf

  Debug build with `TINYGLTF_NO_EXTERNAL_IMAGE ON`
  ```CMake
  set(TINYGLTF_NO_EXTERNAL_IMAGE ON)
  target_compile_definitions(tinygltf PUBLIC TINYGLTF_NO_EXTERNAL_IMAGE)
  ```
  
* **Lua** https://github.com/ThePhD/sol2
  

# Windows

### External dependencies

* glew
  * lib/... (platform binaries)
  * include/GL (includes)
* glm/glm (includes)
* SDL2/x86_64-w64-mingw32
    * lib
    * include
* tinygltf (include files, build from source)
  * json.hpp
  * libtinygltf.a
  * stb_image.h
  * stb_image_write.h
  * tiny_gltf.h
* Jolt (build from source)
  * Jolt - source from the JoltPhysics/Jolt
  * libJolt.a - Built library
* sol2
  * include/sol (includes)
  * liblua-5.4.4.dll.a
  * dll file in the /bin
* lua (same version as for int sol2)
  * include
  * liblua54.a
  
# Ubuntu

### Install

`libglm-dev`
`libsdl2-2.0-0`
`libsdl2-dev`
`libglew-dev`

### External dependencies
* Jolt (build from source)
  * Jolt - source from the JoltPhysics/Jolt
  * libJolt.a - Built library

* tinygltf (include files, build from source)
  * build (libs)
