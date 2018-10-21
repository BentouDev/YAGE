# Add CTTI
#
# Creates and setups command to generate CTTI using Agnes
function (yage_add_ctti NAME)
    set(options TEST)
    set(oneArg PATTERN DIRECTORY)
    set(multiArg OUTPUT DEPENDS HEADERS)
    cmake_parse_arguments(AGNES "${options}" "${oneArg}" "${multiArg}" ${ARGN} )

    string(TOUPPER ${NAME} AGNES_VAR_NAME)

    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Generated/${NAME})

    if (AGNES_DIRECTORY)
        set(DIR_PARAM "-d " ${AGNES_DIRECTORY})
    endif()

    set(INPUT_FILE_CONTENT)
    foreach(HEADER_FILE ${AGNES_HEADERS})
        string (CONCAT INPUT_FILE_CONTENT ${INPUT_FILE_CONTENT} "\n" ${HEADER_FILE})
    endforeach()

    string (CONCAT INPUT_FILE ${CMAKE_SOURCE_DIR} "/" ${AGNES_DIRECTORY} "/headers.txt")
    message("-- yage: Creating Agnes input file: " ${INPUT_FILE})

    file(WRITE ${INPUT_FILE} ${INPUT_FILE_CONTENT})

    add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/Generated/${NAME}/generated.timestamp ${AGNES_OUTPUT}
        DEPENDS ${AGNES_DEPENDS} ${INPUT_FILE} ${AGNES_HEADERS}
        COMMENT "Generating CTTI for ${NAME}..."
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${AGNES_BINARY} ARGS ${DIR_PARAM} ${AGNES_PATTERN} "-i " ${INPUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_SOURCE_DIR}/Generated/${NAME}/generated.timestamp)

    add_custom_target(YAGE_GENERATE_${AGNES_VAR_NAME}_CTTI
            DEPENDS ${CMAKE_SOURCE_DIR}/Generated/${NAME}/generated.timestamp
            COMMENT "Checking if ${NAME} CTTI needs regeneration...")

    file(GLOB
        ${AGNES_VAR_NAME}_GENERATED_CRTTI
        ${CMAKE_SOURCE_DIR}/Generated/${NAME}/*.h)

    add_library(${NAME}_CTTI STATIC ${${AGNES_VAR_NAME}_GENERATED_CRTTI} ${AGNES_OUTPUT})
    add_library(CTTI::${NAME} ALIAS ${NAME}_CTTI)
    add_dependencies(${NAME}_CTTI YAGE_GENERATE_${AGNES_VAR_NAME}_CTTI)
    set_target_properties(${NAME}_CTTI PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 17)

endfunction()

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
    set(options PUBLIC_INSTALL)
    set(oneArg PREFER TARGET SOURCE VAR_NAME CONAN NOT_OS)
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
        if ((NOT DEP_PREFER STREQUAL "BUILD") AND (NOT DEP_NOT_OS))
            find_package(${NAME})
            message("-- yage: Search for " ${NAME} " in OS")
            if (${DEP_VAR_NAME}_FOUND)
                message("--   " ${NAME} " found!")
            else()
                message("--   " ${NAME} " not found!")
            endif()
        endif()

        if ((NOT DEP_PREFER STREQUAL "BUILD") AND ((NOT ${${DEP_VAR_NAME}_FOUND}) OR (DEP_NOT_OS)) AND (DEP_CONAN))
            message ("-- yage: Search for " ${NAME} " in conan repositories")

            conan_cmake_run(REQUIRES ${DEP_CONAN}
                BASIC_SETUP)

            set(${DEP_VAR_NAME}_CONAN_FOUND TRUE)
            set(${DEP_VAR_NAME}_CONAN_FOUND TRUE PARENT_SCOPE)

            if (CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE )

                foreach(CMAKE_BUILD_TYPE "RELEASE" "DEBUG")
                    set(CONAN_${DEP_VAR_NAME}_BINARY ${CONAN_BIN_${DEP_VAR_NAME}_${CMAKE_BUILD_TYPE}} ${CONAN_${DEP_VAR_NAME}_BINARY})
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

                set(CONAN_${DEP_VAR_NAME}_BINARY ${CONAN_BIN_${DEP_VAR_NAME}} )
                set(CONAN_${DEP_VAR_NAME}_BINARY ${CONAN_BIN_${DEP_VAR_NAME}} PARENT_SCOPE)

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
                
                cmake_policy(PUSH)
                cmake_policy(SET CMP0048 OLD)

                    add_subdirectory(${DEP_SOURCE} EXCLUDE_FROM_ALL)

                cmake_policy(POP)

            endif()

            set(${DEP_VAR_NAME}_TARGET ${DEP_TARGET} PARENT_SCOPE)
            set(${DEP_VAR_NAME}_INCLUDE_DIR ${DEP_INCLUDE} PARENT_SCOPE)

            set(${DEP_VAR_NAME}_LIBRARY ${DEP_TARGET} PARENT_SCOPE)
            set(${DEP_VAR_NAME}_LIBRARIES ${DEP_TARGET} PARENT_SCOPE)

        endif()
        # if(DEP_PUBLIC_INSTALL)
        #     message ("-- yage: " ${NAME} " will be publicly exported on install!")
        #     install (DIRECTORY ${DEP_INCLUDE} DESTINATION ${YAGE_INCLUDE_DIR})
        # endif()
    endif()
endfunction()