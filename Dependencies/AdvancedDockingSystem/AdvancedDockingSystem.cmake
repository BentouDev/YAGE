# AdvancedDockingSystem

set (ADS_SOURCES
	Dependencies/AdvancedDockingSystem/Source/src/API.cpp
	Dependencies/AdvancedDockingSystem/Source/src/ContainerWidget.cpp
	Dependencies/AdvancedDockingSystem/Source/src/SectionWidget.cpp
	Dependencies/AdvancedDockingSystem/Source/src/SectionContent.cpp 
	Dependencies/AdvancedDockingSystem/Source/src/SectionTitleWidget.cpp
	Dependencies/AdvancedDockingSystem/Source/src/SectionContentWidget.cpp
	Dependencies/AdvancedDockingSystem/Source/src/DropOverlay.cpp 
	Dependencies/AdvancedDockingSystem/Source/src/FloatingWidget.cpp 
	Dependencies/AdvancedDockingSystem/Source/src/Internal.cpp 
    Dependencies/AdvancedDockingSystem/Source/src/Serialization.cpp
)

set (ADS_HEADERS
	Dependencies/AdvancedDockingSystem/Source/include/ads/API.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/ContainerWidget.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/SectionWidget.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/SectionContent.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/SectionTitleWidget.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/SectionContentWidget.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/DropOverlay.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/FloatingWidget.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/Internal.h 
	Dependencies/AdvancedDockingSystem/Source/include/ads/Serialization.h
)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

yage_include(YAGE Dependencies/AdvancedDockingSystem/Source/include)

add_library(ADS_LIB SHARED ${ADS_SOURCES} ${ADS_HEADERS})
target_compile_definitions(ADS_LIB PRIVATE ADS_EXPORT)
target_link_libraries(ADS_LIB Qt5::Core Qt5::Widgets)

install(TARGETS ADS_LIB
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})