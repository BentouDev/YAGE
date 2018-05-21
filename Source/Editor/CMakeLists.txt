## QT based Editor for YAGE
project(Editor VERSION 0.1 LANGUAGES CXX)

# Automate build of Qt files
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Additional setup for Windows
if(WIN32)
    set(OS_BUNDLE WIN32)
    set(OS_LIBS Qt5::WinExtras)
endif(WIN32)

#qt_

add_executable(Editor ${OS_BUNDLE} src/main.cpp include/EditorWindow.h src/EditorWindow.cpp include/BasePage.h src/BasePage.cpp include/EditorTabBar.h src/EditorTabBar.cpp include/EditorTabWidget.h src/EditorTabWidget.cpp editorresources.qrc include/QProperty.h include/QClassView.h)
add_executable(YAGE::Editor ALIAS Editor)

target_include_directories(Editor PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utils/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_compile_features(Editor
        PUBLIC cxx_auto_type cxx_nullptr cxx_variadic_templates)

# Disable static link of Qt
set_property(TARGET Editor PROPERTY QT5_NO_LINK_QTMAIN ON)

target_link_libraries(Editor YAGE::Engine Qt5::Core Qt5::Widgets ${OS_LIBS})

# 'make install' to the correct location
install(TARGETS Editor
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
# Do not distribute editor headers
# install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS Editor FILE EditorConfig.cmake)
