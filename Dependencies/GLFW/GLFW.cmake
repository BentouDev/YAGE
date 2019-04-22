# GLFW
set (GLFW_BUILD_DOCS OFF CACHE BOOL "")
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set (GLFW_BUILD_TESTS OFF CACHE BOOL "")

yage_setup_dependency(GLFW
        CONAN glfw/3.3-dev@bentoudev/stable
        TARGET glfw
        SOURCE Dependencies/GLFW/Source
        INCLUDE Dependencies/GLFW/Source/include)