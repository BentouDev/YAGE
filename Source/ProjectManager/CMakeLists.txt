## QT based ProjectManager for YAGE
project(ProjectManager VERSION 0.1 LANGUAGES CXX)
set (YAGE_MANAGER_OUTPUT_NAME "YageProjectManager")

# Additional setup for Windows
if(WIN32)
    # set(OS_BUNDLE WIN32)
    set(OS_LIBS Qt5::WinExtras)
    add_definitions(-DWIN32 -D_WINDOWS -D_UNICODE -DUNICODE)
endif(WIN32)

message("-- yage: Collecting headers from " ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE
        MANAGER_ALL_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

set(YAGE_REFLECTION_CONFIG
        ${CMAKE_SOURCE_DIR}/Data/Reflection/main_template.mustache
        ${CMAKE_SOURCE_DIR}/Data/Reflection/reflection_header.mustache
        ${CMAKE_SOURCE_DIR}/Data/Reflection/reflection_unit.mustache
        ${CMAKE_SOURCE_DIR}/Data/Reflection/Yage.json)

set_property(SOURCE ${CMAKE_SOURCE_DIR}/Generated/ProjectManager/register_reflection.cpp PROPERTY SKIP_AUTOGEN ON)

yage_add_ctti(ProjectManager
        DIRECTORY Generated/ProjectManager
        PATTERN Data/Reflection/Editor.json
        OUTPUT ${CMAKE_SOURCE_DIR}/Generated/ProjectManager/register_reflection.cpp
        HEADERS ${MANAGER_ALL_HEADERS}
        DEPENDS ${YAGE_REFLECTION_CONFIG})

# Automate build of Qt files
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_add_resources(MANAGER_QML_RESOURCES resources/qml.qrc)

add_executable(ProjectManager ${MANAGER_QML_RESOURCES} ${OS_BUNDLE} ProjectManager.qrc src/main.cpp include/MainWindow.h src/MainWindow.cpp include/Backend.h src/Backend.cpp include/Project.h src/Project.cpp include/StandardIconProvider.h include/QmlFrame.h src/QmlFrame.cpp)
add_executable(YAGE::ProjectManager ALIAS ProjectManager)
set_target_properties(ProjectManager PROPERTIES OUTPUT_NAME ${YAGE_MANAGER_OUTPUT_NAME})

target_include_directories(ProjectManager PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE src)

target_compile_features(ProjectManager
        PUBLIC cxx_auto_type cxx_nullptr cxx_variadic_templates)

target_compile_definitions(ProjectManager PRIVATE ${YAGE_EXE_COMPILE_DEFS})

# Disable static link of Qt
set_property(TARGET ProjectManager PROPERTY QT5_NO_LINK_QTMAIN ON)

target_link_libraries(ProjectManager Qt5::Core Qt5::Widgets Qt5::Qml Qt5::Quick ${OS_LIBS} CTTI::ProjectManager)

# 'make install' to the correct location
install(TARGETS ProjectManager
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})

# Deploy Qt library
if (WIN32)
        string (CONCAT MANAGER_BINARY ${YAGE_BINARY_DIR}/${YAGE_MANAGER_OUTPUT_NAME} ".exe")
        string (CONCAT MANAGER_DEPLOY "execute_process(COMMAND " ${WINDEPLOYQT_EXECUTABLE} " --qmldir " ${CMAKE_CURRENT_SOURCE_DIR} "/resources " ${MANAGER_BINARY} ")")
        install(CODE ${MANAGER_DEPLOY})
endif()

# This makes the project importable from the build directory
export(TARGETS ProjectManager ProjectManager_CTTI FILE ProjectManagerConfig.cmake)
