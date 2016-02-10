## Simple game engine
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(Volkhvy VERSION 0.1 LANGUAGES CXX)

find_package(Glm REQUIRED)
set_property(TARGET glm PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GLM_INCLUDE_DIR})

add_subdirectory(Dependencies/GoogleTest EXCLUDE_FROM_ALL)

# Include Boost as an imported target
#find_package(Boost REQUIRED)
#add_library(boost INTERFACE IMPORTED)
#set_property(TARGET boost PROPERTY
#        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})


# External 3rd party libs that we include
# libraries to add:
# opengl, glm, squirell, lua, sdl2, bullet
# add_subdirectory(Dependencies/GLM EXCLUDE_FROM_ALL)

# Include all modules
enable_testing()
add_subdirectory(Source/Core)
add_subdirectory(Source/Parser)
add_subdirectory(Source/Script)