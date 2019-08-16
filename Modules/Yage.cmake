# Add CTTI
#
# Creates and setups command to generate CTTI using Agnes
function (yage_add_ctti NAME)
    set(options TEST)
    set(oneArg PATTERN DIRECTORY)
    set(multiArg OUTPUT DEPENDS HEADERS)
    cmake_parse_arguments(AGNES "${options}" "${oneArg}" "${multiArg}" ${ARGN} )

	if (NOT AGNES_BINARY)
		message(FATAL_ERROR "-- yage: Agnes not found!")
	else()
		message("-- yage: Agnes: " ${AGNES_BINARY})
	endif()

    string(TOUPPER ${NAME} AGNES_VAR_NAME)

    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${AGNES_DIRECTORY})

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
        OUTPUT ${CMAKE_SOURCE_DIR}/${AGNES_DIRECTORY}/generated.timestamp ${AGNES_OUTPUT}
        DEPENDS ${AGNES_DEPENDS} ${INPUT_FILE} ${AGNES_HEADERS}
        COMMENT "Generating CTTI for ${NAME}..."
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${AGNES_BINARY} ARGS ${DIR_PARAM} ${AGNES_PATTERN} "-i " ${INPUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_SOURCE_DIR}/${AGNES_DIRECTORY}/generated.timestamp)

    add_custom_target(YAGE_GENERATE_${AGNES_VAR_NAME}_CTTI
        DEPENDS 
            ${CMAKE_SOURCE_DIR}/${AGNES_DIRECTORY}/generated.timestamp 
            ${CMAKE_SOURCE_DIR}/${AGNES_DIRECTORY}/headers.txt
        COMMENT
            "Checking if ${NAME} CTTI needs regeneration...")

    file(GLOB
        ${AGNES_VAR_NAME}_GENERATED_CRTTI
        ${CMAKE_SOURCE_DIR}/${AGNES_DIRECTORY}/*.h
        ${CMAKE_SOURCE_DIR}/${AGNES_DIRECTORY}/*.cpp)

    add_library(${NAME}_CTTI OBJECT ${${AGNES_VAR_NAME}_GENERATED_CRTTI} ${AGNES_OUTPUT})
    add_library(CTTI::${NAME} ALIAS ${NAME}_CTTI)
    add_dependencies(${NAME}_CTTI YAGE_GENERATE_${AGNES_VAR_NAME}_CTTI)
    set_target_properties(${NAME}_CTTI PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 17)

endfunction()

# Glob
#
# Glob files matching given patterns and put them into variable
function(yage_glob_files NAME)
    set(options )
    set(oneArg )
    set(multiArg PATTERNS)
    cmake_parse_arguments(YAGE "${options}" "${oneArg}" "${multiArg}" ${ARGN} )

    file(GLOB_RECURSE
        ${NAME}
        CONFIGURE_DEPENDS
        LIST_DIRECTORIES false
        ${YAGE_PATTERNS})

    # Make this visible outside funtion
    set(${NAME} ${${NAME}} PARENT_SCOPE)
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

# Configure Conan
#
# Conan cmake support is subpar, we can do better
function(yage_configure_conan)

	list(LENGTH CONAN_DEPENDENCIES targets_len_abs)
	math(EXPR targets_len "${targets_len_abs} - 1")

	# We got list of all deps
	# Need to call FindPackage for them
	# Or create separate target's

	set(_config_file_path ${CMAKE_BINARY_DIR}/yage-config.cmake)
	message ("-- yage: WRITTING CONFIG... " ${_config_file_path})

	file(WRITE ${_config_file_path} "set(CMAKE_MODULE_PATH $")
	file(APPEND ${_config_file_path} "{CMAKE_MODULE_PATH} $")
	file(APPEND ${_config_file_path} "{CMAKE_CURRENT_LIST_DIR})\ninclude(CMakeFindDependencyMacro)\n")
    file(APPEND ${_config_file_path} """
# Compute relative path
get_filename_component(_IMPORT_PREFIX "\${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_IMPORT_PREFIX "\${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "\${_IMPORT_PREFIX}" PATH)
if(_IMPORT_PREFIX STREQUAL "/")
    set(_IMPORT_PREFIX "")
endif()
"""
    )

	foreach(idx RANGE ${targets_len})
	  list(GET CONAN_DEPENDENCIES ${idx} _dep)
	  
	  string(TOUPPER ${_dep} _DEP)
	  set(_YAGE_LIB_NAME ${_dep})

	  message("-- yage: Processing conan package " ${_dep} "...")

	  if (EXISTS ${CMAKE_BINARY_DIR}/Find${_dep}.cmake)

		# Cmake find package generator
		message("-- yage: Found package script Find'${_dep}'.cmake!")

		find_package(${_dep} REQUIRED)

		set(_dep_dep ${_dep}::${_dep})

        set(${_dep}_LIB_DIRS ${${_dep}_LIB_DIRS} PARENT_SCOPE)

        set(${_dep}_lib_kind INTERFACE)
		set(_target_lib_file)

        foreach (_lib_path ${${_dep}_LIBS})
            #string(FIND ${_lib_path} ".dll" _is_dll_pos REVERSE)
            #string(FIND ${_lib_path} ".lib" _is_lib_pos REVERSE)

            get_filename_component(_lib_file ${_lib_path} NAME)
            get_filename_component(_lib_ext ${_lib_path} EXT)

            if (_lib_ext)
                
                message ("-- yage: DEBUG: " ${_lib_file} " and " ${_lib_ext})

                string(FIND ".dll .so" ${_lib_ext} _is_dynamic_pos REVERSE)
                string(FIND ".lib .d" ${_lib_ext} _is_static_pos REVERSE)

			    if (NOT (${_is_dynamic_pos} EQUAL -1))
				    set(${_dep}_lib_kind SHARED)
				    set(_target_lib_file "\${_IMPORT_PREFIX}//${YAGE_BINARY_DIR}//${_lib_file}")
			    endif()

			    if (${${_dep}_lib_kind} STREQUAL "INTERFACE")
				    if(NOT (${_is_static_pos} EQUAL -1))
					    set(${_dep}_lib_kind STATIC)
					    set(_target_lib_file "\${_IMPORT_PREFIX}//${YAGE_LIBRARY_DIR}//${_lib_file}")
				    endif()
			    endif()
            endif()

        endforeach()

        message("-- yage: Lib ${_dep} is a ${${_dep}_lib_kind} library ")

        if (NOT TARGET _dep_dep)
            add_library(Yage::DEP::${_YAGE_LIB_NAME} INTERFACE IMPORTED)

            target_link_libraries(Yage::DEP::${_YAGE_LIB_NAME} INTERFACE ${_dep_dep})

            file(APPEND ${_config_file_path} "add_library(Yage::DEP::${_YAGE_LIB_NAME} ${${_dep}_lib_kind} IMPORTED)\n")
            if (_target_lib_file)
                file(APPEND ${_config_file_path} "set_target_properties(Yage::DEP::${_YAGE_LIB_NAME} PROPERTIES IMPORTED_LOCATION ${_target_lib_file})\n")
            endif()

            # Get only first directory specified
            list(GET ${_dep}_INCLUDE_DIRS "0" _include_header)
            if (_include_header)
                if (${_include_header} MATCHES "include$")
                    SET(_header_dir /)
                else()
                    SET(_header_dir /include)
                endif()

                install(DIRECTORY ${_include_header} DESTINATION ${_header_dir})
                target_include_directories(Yage::DEP::${_YAGE_LIB_NAME} INTERFACE 
                    $<BUILD_INTERFACE:${_include_header}>
                    $<INSTALL_INTERFACE:${_header_dir}>)
            endif()

		endif()

	  else()
		# Classic target setup
		set(_dir ${CONAN_LIB_DIRS_${_DEP}})
		message("-- yage: Looking for Conan target '${_dep}' location: ${_dir}")
		find_library(_found_lib NAME ${_dep} PATHS ${_dir} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

		if (_found_lib)
			set(_imported_location ${_dir}/${_dep})
			add_library(${_YAGE_LIB_NAME} UNKNOWN IMPORTED)
			add_library(Yage::${_YAGE_LIB_NAME} ALIAS ${_YAGE_LIB_NAME})
			set_target_properties(${_YAGE_LIB_NAME} PROPERTIES IMPORTED_LOCATION ${_imported_location})
			#add_library( SHARED IMPORTED)
			#set_target_properties(${_target} PROPERTIES IMPORTED_LOCATION ${_imported_location})
			message("-- yage: Automatic Conan target '${_dep}' install configure: ${_imported_location}")

			INSTALL(TARGETS ${_YAGE_LIB_NAME} RUNTIME DESTINATION ${YAGE_BINARY_DIR})
		else()
			message("-- yage: Error: Unable to configure Conan target '${_dep}'")
		endif()
	  endif()

	  set(_YAGE_LIB_NAME)
	  set(_DEP)

	endforeach()

	file(APPEND ${_config_file_path} "include($")
	file(APPEND ${_config_file_path} "{CMAKE_CURRENT_LIST_DIR}")
	file(APPEND ${_config_file_path} "/yage-targets.cmake)")
	install(FILES ${_config_file_path} DESTINATION "${YAGE_LIBRARY_DIR}")

    foreach(_lib_itr ${CONAN_LIB_DIRS})
        install(DIRECTORY ${_lib_itr}/ DESTINATION ${YAGE_LIBRARY_DIR})
    endforeach()

    install(DIRECTORY ${CONAN_BIN_DIRS} DESTINATION ${YAGE_BINARY_DIR})
    
endfunction()

# Setup Dependency
#
# Provides procedure to easily setup dependency that can both be build from source or get from environment
function(yage_setup_dependency NAME)
    set(options PUBLIC_INSTALL)
    set(oneArg PREFER TARGET SOURCE VAR_NAME CONAN NOT_OS OS_AFTER_CONAN)
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

			if (NOT ${CONAN_${DEP_VAR_NAME}_ROOT})
				conan_cmake_run(REQUIRES ${DEP_CONAN}
					BASIC_SETUP BUILD missing GENERATORS cmake_find_package)
			endif()

            set(${DEP_VAR_NAME}_CONAN_FOUND TRUE)
            set(${DEP_VAR_NAME}_CONAN_FOUND TRUE CACHE BOOL "Is dependency found by Conan")

            if (CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE )

                foreach(CMAKE_BUILD_TYPE "RELEASE" "DEBUG")
                    set(CONAN_${DEP_VAR_NAME}_BINARY ${CONAN_BIN_${DEP_VAR_NAME}_${CMAKE_BUILD_TYPE}} ${CONAN_${DEP_VAR_NAME}_BINARY} CACHE FILEPATH "" FORCE)
                    set(CONAN_${DEP_VAR_NAME}_LIBRARIES ${CONAN_LIBS_${DEP_VAR_NAME}_${CMAKE_BUILD_TYPE}} ${CONAN_${DEP_VAR_NAME}_LIBRARIES} CACHE STRING "" FORCE)
                    set(CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR ${CONAN_LIB_DIRS_${DEP_VAR_NAME}_${CMAKE_BUILD_TYPE}} ${CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR} CACHE PATH "" FORCE)
                    set(CONAN_${DEP_VAR_NAME}_INCLUDE_DIR ${CONAN_INCLUDE_DIRS_${DEP_VAR_NAME}_${CMAKE_BUILD_TYPE}} ${CONAN_${DEP_VAR_NAME}_INCLUDE_DIR} CACHE PATH "" FORCE)
                endforeach()

                #set(${DEP_VAR_NAME}_LIBRARIES  PARENT_SCOPE)
                #set(${DEP_VAR_NAME}_LIBRARIES_DIR ${CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR} PARENT_SCOPE)
                set(${DEP_VAR_NAME}_INCLUDE_DIR ${CONAN_${DEP_VAR_NAME}_INCLUDE_DIR} CACHE PATH "" FORCE)

                set(CMAKE_BUILD_TYPE)

            else()
                #set(${DEP_VAR_NAME}_LIBRARY ${CONAN_LIBS_${DEP_VAR_NAME}} )
                #set(${DEP_VAR_NAME}_LIBRARY ${CONAN_LIBS_${DEP_VAR_NAME}} PARENT_SCOPE)

                set(CONAN_${DEP_VAR_NAME}_BINARY ${CONAN_BIN_${DEP_VAR_NAME}} )
                set(CONAN_${DEP_VAR_NAME}_BINARY ${CONAN_BIN_${DEP_VAR_NAME}} CACHE FILEPATH "" FORCE)

                set(CONAN_${DEP_VAR_NAME}_LIBRARIES ${CONAN_LIBS_${DEP_VAR_NAME}} )
                set(CONAN_${DEP_VAR_NAME}_LIBRARIES ${CONAN_LIBS_${DEP_VAR_NAME}} CACHE STRING "" FORCE)

                set(CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR ${CONAN_LIB_DIRS_${DEP_VAR_NAME}} )
                set(CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR ${CONAN_LIB_DIRS_${DEP_VAR_NAME}} CACHE PATH "" FORCE)

                set(${DEP_VAR_NAME}_INCLUDE_DIR ${CONAN_INCLUDE_DIRS_${DEP_VAR_NAME}} )
                set(${DEP_VAR_NAME}_INCLUDE_DIR ${CONAN_INCLUDE_DIRS_${DEP_VAR_NAME}} CACHE PATH "" FORCE)
            endif()

            set(${DEP_VAR_NAME}_LIBRARIES)
            set(${DEP_VAR_NAME}_LIBRARIES PARENT_SCOPE)

			set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${CONAN_LIBS})

			list(LENGTH CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR _LIB_DIR_COUNT)
            if (_LIB_DIR_COUNT)
                message("-- yage: Conan package library search dir : " ${CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR})
                foreach(_LIB_NAME ${CONAN_${DEP_VAR_NAME}_LIBRARIES})
                    find_library(YAGE_FOUND_LIBRARY NAMES ${_LIB_NAME} PATHS ${CONAN_${DEP_VAR_NAME}_LIBRARIES_DIR}
                        NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

                    if (YAGE_FOUND_LIBRARY)
                        message ("--   Found packaged library " ${_LIB_NAME})
                        set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${YAGE_FOUND_LIBRARY} )
                        set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${YAGE_FOUND_LIBRARY} CACHE STRING "" FORCE)
                    else()
                        message ("--   Will attempt to use OS library " ${_LIB_NAME})
                        set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${_LIB_NAME} )
                        set(${DEP_VAR_NAME}_LIBRARIES ${${DEP_VAR_NAME}_LIBRARIES} ${_LIB_NAME} CACHE STRING "" FORCE)
                    endif()

                    unset(YAGE_FOUND_LIBRARY CACHE)
                endforeach()
            endif()
			
			message ("--yage: Found libs: " ${${DEP_VAR_NAME}_LIBRARIES})

        endif()

        if (DEP_OS_AFTER_CONAN)
            find_package(${NAME})
            message("-- yage: Repeat search for " ${NAME} " in OS after Conan install...")
            if (${DEP_VAR_NAME}_FOUND)
                message("--   " ${NAME} " found!")
            else()
                message("--   " ${NAME} " not found!")
            endif()
        endif()

        if ((DEP_PREFER STREQUAL "BUILD") OR ((NOT ${DEP_VAR_NAME}_FOUND) AND (NOT ${DEP_VAR_NAME}_CONAN_FOUND)))
            
			if (DEP_INCLUDE OR DEP_SOURCE)
				if (DEP_TARGET STREQUAL NAME)
					message ("-- yage: Building " ${NAME} " from source!")
				else()
					message ("-- yage: Building " ${NAME} " from source as target " ${DEP_TARGET} "!")
				endif()
			else()
				message(FATAL_ERROR "-- yage: Failrue! " ${NAME} " not found...")
			endif()

            if (DEP_INCLUDE)
                foreach (incl IN ITEMS ${DEP_INCLUDE})
                    message("--   " ${NAME} " include dir " ${incl})
                endforeach()
            endif()

            if (DEP_SOURCE)
                message("--   " ${NAME} " source dir " ${DEP_SOURCE})
                

                    add_subdirectory(${DEP_SOURCE} EXCLUDE_FROM_ALL)

					set(${DEP_VAR_NAME}_FROM_SOURCE True CACHE STRING "" FORCE)
            endif()

            set(${DEP_VAR_NAME}_TARGET ${DEP_TARGET} CACHE STRING "" FORCE)
            set(${DEP_VAR_NAME}_INCLUDE_DIR ${DEP_INCLUDE} CACHE PATH "" FORCE)

            set(${DEP_VAR_NAME}_LIBRARY ${DEP_TARGET} CACHE FILEPATH "" FORCE)
            set(${DEP_VAR_NAME}_LIBRARIES ${DEP_TARGET} CACHE STRING "" FORCE)

        endif()
        # if(DEP_PUBLIC_INSTALL)
        #     message ("-- yage: " ${NAME} " will be publicly exported on install!")
        #     install (DIRECTORY ${DEP_INCLUDE} DESTINATION ${YAGE_INCLUDE_DIR})
        # endif()
    endif()

    #message("-- yage: TARGET - " ${${DEP_VAR_NAME}_TARGET})
    #message("-- yage: INCLUDE_DIR - " ${${DEP_VAR_NAME}_INCLUDE_DIR})
    #message("-- yage: LIBRARY - " ${${DEP_VAR_NAME}_LIBRARY})
    #message("-- yage: LIBRARIES - " ${${DEP_VAR_NAME}_LIBRARIES})

endfunction()