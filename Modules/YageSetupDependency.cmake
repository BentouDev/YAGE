# Include
#
# Includes directories both in current scope and for target
function(yage_include NAME)
    target_include_directories(${NAME} INTERFACE ${ARGN})
    include_directories(${ARGN})
endfunction()

# Add Dependency
#
# Manages inclusion of cmake file and setting module path
function(yage_add_dependency PATH)
    get_filename_component(NAME ${PATH} NAME)

    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${PATH} ${CMAKE_MODULE_PATH} PARENT_SCOPE)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${PATH} ${CMAKE_MODULE_PATH} )

    include(${PATH}/${NAME}.cmake)
endfunction()

# Setup Dependency
#
# Provides procedure to easily setup dependency that can both be build from source or get from environment
function(yage_setup_dependency NAME)
    set(options TEST)
    set(oneArg PREFER TARGET SOURCE VAR_NAME CONAN)
    set(multiArg INCLUDE)
    cmake_parse_arguments(DEP "${options}" "${oneArg}" "${multiArg}" ${ARGN} )

    if (NOT DEP_TARGET)
        set (DEP_TARGET ${NAME})
    endif()

    if (NOT DEP_VAR_NAME)
        set (DEP_VAR_NAME ${NAME})
    endif()

    if (NOT NAME)
        message(FATAL_ERROR "yage_setup_dependency called without specyfing name!\n")
    else()
        if (NOT DEP_PREFER STREQUAL "BUILD")
            find_package(${NAME})
            message("-- yage: Search for " ${NAME} " in OS")
            if (${DEP_VAR_NAME}_FOUND)
                message("--   " ${NAME} " found!")
            else()
                message("--   " ${NAME} " not found!")
            endif()
        endif()

        if ((NOT DEP_PREFER STREQUAL "BUILD") AND (NOT ${${DEP_VAR_NAME}_FOUND}) AND (DEP_CONAN))
            message ("-- yage: Search for " ${NAME} " in conan repositories")

            conan_cmake_run(REQUIRES ${DEP_CONAN}
                BASIC_SETUP)

            set(${DEP_VAR_NAME}_CONAN_FOUND TRUE)
            set(${DEP_VAR_NAME}_CONAN_FOUND TRUE PARENT_SCOPE)

            if (CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE )

                foreach(CMAKE_BUILD_TYPE "RELEASE" "DEBUG")
                    set(CONAN_${DEP_VAR_NAME}_LIBRARIES ${CONAN_LIBS_${DEP_VAR_NAME}_${CMAKE_BUILD_TYPE}} ${CONAN_${DEP_VAR_NAME}_LIBRARIES})
                    set(CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR ${CONAN_LIB_DIRS_${DEP_VAR_NAME}_${CMAKE_BUILD_TYPE}} ${CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR})
                    set(CONAN_${DEP_VAR_NAME}_INCLUDE_DIR ${CONAN_INCLUDE_DIRS_${DEP_VAR_NAME}_${CMAKE_BUILD_TYPE}} ${CONAN_${DEP_VAR_NAME}_INCLUDE_DIR})
                endforeach()

                #set(${DEP_VAR_NAME}_LIBRARIES  PARENT_SCOPE)
                #set(${DEP_VAR_NAME}_LIBRARIES_DIR ${CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR} PARENT_SCOPE)
                set(${DEP_VAR_NAME}_INCLUDE_DIR ${CONAN_${DEP_VAR_NAME}_INCLUDE_DIR} PARENT_SCOPE)

                set(CMAKE_BUILD_TYPE)

            else()
                #set(${DEP_VAR_NAME}_LIBRARY ${CONAN_LIBS_${DEP_VAR_NAME}} )
                #set(${DEP_VAR_NAME}_LIBRARY ${CONAN_LIBS_${DEP_VAR_NAME}} PARENT_SCOPE)

                set(CONAN_${DEP_VAR_NAME}_LIBRARIES ${CONAN_LIBS_${DEP_VAR_NAME}} )
                set(CONAN_${DEP_VAR_NAME}_LIBRARIES ${CONAN_LIBS_${DEP_VAR_NAME}} PARENT_SCOPE)

                set(CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR ${CONAN_LIB_DIRS_${DEP_VAR_NAME}} )
                set(CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR ${CONAN_LIB_DIRS_${DEP_VAR_NAME}} PARENT_SCOPE)

                set(${DEP_VAR_NAME}_INCLUDE_DIR ${CONAN_INCLUDE_DIRS_${DEP_VAR_NAME}} )
                set(${DEP_VAR_NAME}_INCLUDE_DIR ${CONAN_INCLUDE_DIRS_${DEP_VAR_NAME}} PARENT_SCOPE)
            endif()

            set(${DEP_VAR_NAME}_LIBRARIES)
            set(${DEP_VAR_NAME}_LIBRARIES PARENT_SCOPE)

            message("-- yage: Conan package library search dir " ${CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR})
            foreach(_LIB_NAME ${CONAN_${DEP_VAR_NAME}_LIBRARIES})
                find_library(YAGE_FOUND_LIBRARY NAMES ${_LIB_NAME} PATHS ${CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR}
                    NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

                if (YAGE_FOUND_LIBRARY)
                    message ("--   Found packaged library " ${_LIB_NAME})
                    set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${YAGE_FOUND_LIBRARY} )
                    set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${YAGE_FOUND_LIBRARY} PARENT_SCOPE)
                else()
                    message ("--   Will attempt to use OS library " ${_LIB_NAME})
                    set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${_LIB_NAME} )
                    set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${_LIB_NAME} PARENT_SCOPE)
                endif()

                unset(YAGE_FOUND_LIBRARY CACHE)
            endforeach()

        endif()

        if ((DEP_PREFER STREQUAL "BUILD") OR ((NOT ${DEP_VAR_NAME}_FOUND) AND (NOT ${DEP_VAR_NAME}_CONAN_FOUND)))
            if (DEP_TARGET STREQUAL NAME)
                message ("-- yage: Building " ${NAME} " from source!")
            else()
                message ("-- yage: Building " ${NAME} " from source as target " ${DEP_TARGET} "!")
            endif()

            if (DEP_INCLUDE)
                foreach (incl IN ITEMS ${DEP_INCLUDE})
                    message("--   " ${NAME} " include dir " ${incl})
                endforeach()
            endif()

            if (DEP_SOURCE)
                message("--   " ${NAME} " source dir " ${DEP_SOURCE})

                add_subdirectory(${DEP_SOURCE} EXCLUDE_FROM_ALL)
            endif()

            set(${DEP_VAR_NAME}_TARGET ${DEP_TARGET} PARENT_SCOPE)
            set(${DEP_VAR_NAME}_INCLUDE_DIR ${DEP_INCLUDE} PARENT_SCOPE)

            set(${DEP_VAR_NAME}_LIBRARY ${DEP_TARGET} PARENT_SCOPE)
            set(${DEP_VAR_NAME}_LIBRARIES ${DEP_TARGET} PARENT_SCOPE)

        endif()
    endif()
endfunction()