project(Core VERSION 0.1 LANGUAGES CXX)

## Use all the c++ files we found under this folder for this lib
## I just hate doing it manually
file( GLOB_RECURSE CORE_SRCS
    "Source/*.hxx"
    "Source/*.hpp"
    "Source/*.hh"
    "Source/*.h"
    "Source/*.cxx"
    "Source/*.cpp"
    "Source/*.cc"
    "Source/*.c"
)

add_library(Core ${CORE_SRCS})

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(Core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE Source)

# If we have compiler requirements for this library, list them
# here
target_compile_features(Core
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates)

# Depend on a library that we defined in the top-level file
#target_link_libraries(Core
#        boost)

# 'make install' to the correct location
install(TARGETS Core
        ARCHIVE  DESTINATION Core
        LIBRARY  DESTINATION Core
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS Core FILE CoreConfig.cmake)

# Every library has unit tests, of course
add_executable(CoreTests
        Tests/MeshTest.cpp)

target_link_libraries(CoreTests
        Core)

add_test(CoreTests CoreTests)