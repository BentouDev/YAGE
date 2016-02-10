## Simple game engine
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(Volkhvy VERSION 0.1 LANGUAGES CXX)

# Include Boost as an imported target
#find_package(Boost REQUIRED)
#add_library(boost INTERFACE IMPORTED)
#set_property(TARGET boost PROPERTY
#        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})


# External 3rd party libs that we include
# libraries to add:
# opengl, glm, squirell, lua, sdl2, bullet
#add_subdirectory(Dependencies/glm EXCLUDE_FROM_ALL)

# Include all modules
enable_testing()
add_subdirectory(Source/Core)
add_subdirectory(Source/Parser)
add_subdirectory(Source/Script)