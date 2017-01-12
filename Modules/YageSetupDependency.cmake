# Setup Dependency
#
# Provides procedure to easily setup dependency that can both be build from source or get from environment

function(yage_setup_dependency NAME MODE) # Source Directory, Include Directory, Target name
    if(MODE STREQUAL "SOURCE")
        # provide option to build from source or get from os
        option(YAGE_USE_BUNDLED_${NAME} "Build ${NAME} from source" ON)
    endif()

    if((NOT YAGE_USE_BUNDLED_${NAME}) OR (MODE STREQUAL "OS"))
        # use find_package to get this target from OS
        message("-- Get " ${NAME} " from OS")
        find_package(${NAME})

    elseif(YAGE_USE_BUNDLED_${NAME})
            # build this target from bundled source
            message("-- Build " ${NAME} " from source")

            if(ARGC GREATER 4)

                message("--   " ${NAME} " source dir " ${ARGV2}
                        "\n--   " ${NAME} " include dir " ${ARGV3}
                        "\n--   " ${NAME} " target name " ${ARGV4})

                add_subdirectory(${ARGV2} EXCLUDE_FROM_ALL)
                set(${NAME}_TARGET ${ARGV4} PARENT_SCOPE)
                set(${NAME}_INCLUDE_DIR ${ARGV3} PARENT_SCOPE)

                set(${NAME}_LIBRARY ${ARGV4} PARENT_SCOPE)
                set(${NAME}_LIBRARIES ${ARGV4} PARENT_SCOPE)

            # source dir is specified
            elseif(ARGC GREATER 2)
                message("--   " ${NAME} " source dir " ${ARGV2})
                add_subdirectory(${ARGV2} EXCLUDE_FROM_ALL)
            endif()

    elseif(NOT (MODE STREQUAL "BUNDLE"))
        message(FATAL_ERROR "yage_setup_dependency called with unknown argument(s):\n  " ${MODE})
    endif()
endfunction()
