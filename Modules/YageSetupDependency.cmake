# Setup Dependency
#
# Provides procedure to easily setup dependency that can both be build from source or get from environment
function(yage_setup_dependency NAME)
    set(options TEST)
    set(oneArg PREFER TARGET SOURCE VAR_NAME)
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
        if (NOT DEP_PREFER STREQUAL "SOURCE")
            find_package(${NAME})
            message("-- Search for " ${NAME} " in OS")
            if (${DEP_VAR_NAME}_FOUND)
                message("--   " ${NAME} " found!")
            else()
                message("--   " ${NAME} " not found!")
            endif()
        endif()

        if ((DEP_PREFER STREQUAL "BUILD") OR (NOT ${DEP_VAR_NAME}_FOUND))
            if (DEP_TARGET STREQUAL NAME)
                message ("-- building " ${NAME} " from source!")
            else()
                message ("-- building " ${NAME} " from source as target " ${DEP_TARGET} "!")
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