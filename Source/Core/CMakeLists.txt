## Provides basic classes, like meshes, shaders, materials
project(Core VERSION 0.1 LANGUAGES CXX)

add_library(Core Source/Resources/Mesh.cpp Source/Resources/Mesh.h Source/Window.cpp Source/Window.h Source/Platform.h Source/Viewport.cpp Source/Viewport.h Source/InputManager.cpp Source/InputManager.h Source/Resources/Resource.h Source/Resources/ResourceManager.h Source/Resources/ResourceHandle.h)
add_library(Volkhvy::Core ALIAS Core)

target_include_directories(Core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE Source)

target_compile_features(Core
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates
        cxx_nullptr)

target_link_libraries(Core glm glfw)

# 'make install' to the correct location
install(TARGETS Core
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS glfw Core FILE CoreConfig.cmake)

# Every library has unit tests, of course
add_executable(CoreTests
        Tests/CoreTests.cpp Tests/WindowTests.cpp)

target_link_libraries(CoreTests
        Core glfw gtest gtest_main)

add_test(CoreTests CoreTests)