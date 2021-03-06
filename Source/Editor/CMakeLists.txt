## QT based Editor for YAGE
project(Editor VERSION 0.1 LANGUAGES CXX)

# Additional setup for Windows
if(WIN32)
    set(OS_BUNDLE WIN32)
    set(OS_LIBS Qt5::WinExtras)
    add_definitions(-DWIN32 -D_WINDOWS -D_UNICODE -DUNICODE)
endif(WIN32)

message("-- yage: Collecting headers from " ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE
        EDITOR_ALL_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

set(YAGE_REFLECTION_CONFIG
        ${CMAKE_SOURCE_DIR}/Data/Reflection/main_template.mustache
        ${CMAKE_SOURCE_DIR}/Data/Reflection/reflection_header.mustache
        ${CMAKE_SOURCE_DIR}/Data/Reflection/reflection_unit.mustache
        ${CMAKE_SOURCE_DIR}/Data/Reflection/Yage.json)

set_property(SOURCE ${CMAKE_SOURCE_DIR}/Generated/Editor/register_reflection.cpp PROPERTY SKIP_AUTOGEN ON)

yage_add_ctti(Editor
        DIRECTORY Generated/Editor
        PATTERN Data/Reflection/Editor.json
        OUTPUT ${CMAKE_SOURCE_DIR}/Generated/Editor/register_reflection.cpp
        HEADERS ${EDITOR_ALL_HEADERS}
        DEPENDS ${YAGE_REFLECTION_CONFIG})

# Automate build of Qt files
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(Editor ${OS_BUNDLE} src/main.cpp include/EditorWindow.h src/EditorWindow.cpp include/BasePage.h src/BasePage.cpp include/EditorTabBar.h src/EditorTabBar.cpp include/EditorTabWidget.h src/EditorTabWidget.cpp editorresources.qrc include/QProperty.h include/QClassView.h)
add_executable(YAGE::Editor ALIAS Editor)
set_target_properties(Editor PROPERTIES OUTPUT_NAME "YageEditor")

target_include_directories(Editor PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utils/include>
        PRIVATE src)

target_compile_features(Editor
        PUBLIC cxx_auto_type cxx_nullptr cxx_variadic_templates)

target_compile_definitions(Editor PRIVATE ${YAGE_EXE_COMPILE_DEFS})

# Disable static link of Qt
set_property(TARGET Editor PROPERTY QT5_NO_LINK_QTMAIN ON)

target_link_libraries(Editor Qt5::Core Qt5::Widgets ${OS_LIBS} CTTI::Editor)
target_link_libraries(Editor YAGE::Engine)

# 'make install' to the correct location
install(TARGETS Editor
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})

# This makes the project importable from the build directory
export(TARGETS Editor Editor_CTTI FILE EditorConfig.cmake)
