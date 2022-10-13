########## MACROS ###########################################################################
#############################################################################################

function(conan_message MESSAGE_OUTPUT)
    if(NOT CONAN_CMAKE_SILENT_OUTPUT)
        message(${ARGV${0}})
    endif()
endfunction()


macro(conan_find_apple_frameworks FRAMEWORKS_FOUND FRAMEWORKS FRAMEWORKS_DIRS)
    if(APPLE)
        foreach(_FRAMEWORK ${FRAMEWORKS})
            # https://cmake.org/pipermail/cmake-developers/2017-August/030199.html
            find_library(CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND NAMES ${_FRAMEWORK} PATHS ${FRAMEWORKS_DIRS} CMAKE_FIND_ROOT_PATH_BOTH)
            if(CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND)
                list(APPEND ${FRAMEWORKS_FOUND} ${CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND})
            else()
                message(FATAL_ERROR "Framework library ${_FRAMEWORK} not found in paths: ${FRAMEWORKS_DIRS}")
            endif()
        endforeach()
    endif()
endmacro()


function(conan_package_library_targets libraries package_libdir deps out_libraries out_libraries_target build_type package_name)
    unset(_CONAN_ACTUAL_TARGETS CACHE)
    unset(_CONAN_FOUND_SYSTEM_LIBS CACHE)
    foreach(_LIBRARY_NAME ${libraries})
        find_library(CONAN_FOUND_LIBRARY NAMES ${_LIBRARY_NAME} PATHS ${package_libdir}
                     NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
        if(CONAN_FOUND_LIBRARY)
            conan_message(STATUS "Library ${_LIBRARY_NAME} found ${CONAN_FOUND_LIBRARY}")
            list(APPEND _out_libraries ${CONAN_FOUND_LIBRARY})
            if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
                # Create a micro-target for each lib/a found
                string(REGEX REPLACE "[^A-Za-z0-9.+_-]" "_" _LIBRARY_NAME ${_LIBRARY_NAME})
                set(_LIB_NAME CONAN_LIB::${package_name}_${_LIBRARY_NAME}${build_type})
                if(NOT TARGET ${_LIB_NAME})
                    # Create a micro-target for each lib/a found
                    add_library(${_LIB_NAME} UNKNOWN IMPORTED)
                    set_target_properties(${_LIB_NAME} PROPERTIES IMPORTED_LOCATION ${CONAN_FOUND_LIBRARY})
                    set(_CONAN_ACTUAL_TARGETS ${_CONAN_ACTUAL_TARGETS} ${_LIB_NAME})
                else()
                    conan_message(STATUS "Skipping already existing target: ${_LIB_NAME}")
                endif()
                list(APPEND _out_libraries_target ${_LIB_NAME})
            endif()
            conan_message(STATUS "Found: ${CONAN_FOUND_LIBRARY}")
        else()
            conan_message(STATUS "Library ${_LIBRARY_NAME} not found in package, might be system one")
            list(APPEND _out_libraries_target ${_LIBRARY_NAME})
            list(APPEND _out_libraries ${_LIBRARY_NAME})
            set(_CONAN_FOUND_SYSTEM_LIBS "${_CONAN_FOUND_SYSTEM_LIBS};${_LIBRARY_NAME}")
        endif()
        unset(CONAN_FOUND_LIBRARY CACHE)
    endforeach()

    if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
        # Add all dependencies to all targets
        string(REPLACE " " ";" deps_list "${deps}")
        foreach(_CONAN_ACTUAL_TARGET ${_CONAN_ACTUAL_TARGETS})
            set_property(TARGET ${_CONAN_ACTUAL_TARGET} PROPERTY INTERFACE_LINK_LIBRARIES "${_CONAN_FOUND_SYSTEM_LIBS};${deps_list}")
        endforeach()
    endif()

    set(${out_libraries} ${_out_libraries} PARENT_SCOPE)
    set(${out_libraries_target} ${_out_libraries_target} PARENT_SCOPE)
endfunction()


########### FOUND PACKAGE ###################################################################
#############################################################################################

include(FindPackageHandleStandardArgs)

conan_message(STATUS "Conan: Using autogenerated FindWebP.cmake")
set(WebP_FOUND 1)
set(WebP_VERSION "1.2.2")

find_package_handle_standard_args(WebP REQUIRED_VARS
                                  WebP_VERSION VERSION_VAR WebP_VERSION)
mark_as_advanced(WebP_FOUND WebP_VERSION)

set(WebP_COMPONENTS WebP::webpdecoder WebP::webpdemux WebP::libwebpmux WebP::webp)

if(WebP_FIND_COMPONENTS)
    foreach(_FIND_COMPONENT ${WebP_FIND_COMPONENTS})
        list(FIND WebP_COMPONENTS "WebP::${_FIND_COMPONENT}" _index)
        if(${_index} EQUAL -1)
            conan_message(FATAL_ERROR "Conan: Component '${_FIND_COMPONENT}' NOT found in package 'WebP'")
        else()
            conan_message(STATUS "Conan: Component '${_FIND_COMPONENT}' found in package 'WebP'")
        endif()
    endforeach()
endif()

########### VARIABLES #######################################################################
#############################################################################################


set(WebP_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_INCLUDES "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_RES_DIRS )
set(WebP_DEFINITIONS )
set(WebP_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)
set(WebP_COMPILE_DEFINITIONS )
set(WebP_COMPILE_OPTIONS_LIST "" "")
set(WebP_COMPILE_OPTIONS_C "")
set(WebP_COMPILE_OPTIONS_CXX "")
set(WebP_LIBRARIES_TARGETS "") # Will be filled later, if CMake 3
set(WebP_LIBRARIES "") # Will be filled later
set(WebP_LIBS "") # Same as WebP_LIBRARIES
set(WebP_SYSTEM_LIBS )
set(WebP_FRAMEWORK_DIRS )
set(WebP_FRAMEWORKS )
set(WebP_FRAMEWORKS_FOUND "") # Will be filled later
set(WebP_BUILD_MODULES_PATHS )

conan_find_apple_frameworks(WebP_FRAMEWORKS_FOUND "${WebP_FRAMEWORKS}" "${WebP_FRAMEWORK_DIRS}")

mark_as_advanced(WebP_INCLUDE_DIRS
                 WebP_INCLUDE_DIR
                 WebP_INCLUDES
                 WebP_DEFINITIONS
                 WebP_LINKER_FLAGS_LIST
                 WebP_COMPILE_DEFINITIONS
                 WebP_COMPILE_OPTIONS_LIST
                 WebP_LIBRARIES
                 WebP_LIBS
                 WebP_LIBRARIES_TARGETS)

# Find the real .lib/.a and add them to WebP_LIBS and WebP_LIBRARY_LIST
set(WebP_LIBRARY_LIST webpdecoder webpdemux webpmux webp)
set(WebP_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/lib")

# Gather all the libraries that should be linked to the targets (do not touch existing variables):
set(_WebP_DEPENDENCIES "${WebP_FRAMEWORKS_FOUND} ${WebP_SYSTEM_LIBS} ")

conan_package_library_targets("${WebP_LIBRARY_LIST}"  # libraries
                              "${WebP_LIB_DIRS}"      # package_libdir
                              "${_WebP_DEPENDENCIES}"  # deps
                              WebP_LIBRARIES            # out_libraries
                              WebP_LIBRARIES_TARGETS    # out_libraries_targets
                              ""                          # build_type
                              "WebP")                                      # package_name

set(WebP_LIBS ${WebP_LIBRARIES})

foreach(_FRAMEWORK ${WebP_FRAMEWORKS_FOUND})
    list(APPEND WebP_LIBRARIES_TARGETS ${_FRAMEWORK})
    list(APPEND WebP_LIBRARIES ${_FRAMEWORK})
endforeach()

foreach(_SYSTEM_LIB ${WebP_SYSTEM_LIBS})
    list(APPEND WebP_LIBRARIES_TARGETS ${_SYSTEM_LIB})
    list(APPEND WebP_LIBRARIES ${_SYSTEM_LIB})
endforeach()

# We need to add our requirements too
set(WebP_LIBRARIES_TARGETS "${WebP_LIBRARIES_TARGETS};")
set(WebP_LIBRARIES "${WebP_LIBRARIES};")

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH})


########### COMPONENT webp VARIABLES #############################################

set(WebP_webp_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webp_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webp_INCLUDES "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webp_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/lib")
set(WebP_webp_RES_DIRS )
set(WebP_webp_DEFINITIONS )
set(WebP_webp_COMPILE_DEFINITIONS )
set(WebP_webp_COMPILE_OPTIONS_C "")
set(WebP_webp_COMPILE_OPTIONS_CXX "")
set(WebP_webp_LIBS webp)
set(WebP_webp_SYSTEM_LIBS )
set(WebP_webp_FRAMEWORK_DIRS )
set(WebP_webp_FRAMEWORKS )
set(WebP_webp_BUILD_MODULES_PATHS )
set(WebP_webp_DEPENDENCIES )
set(WebP_webp_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)

########### COMPONENT libwebpmux VARIABLES #############################################

set(WebP_libwebpmux_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_libwebpmux_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_libwebpmux_INCLUDES "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_libwebpmux_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/lib")
set(WebP_libwebpmux_RES_DIRS )
set(WebP_libwebpmux_DEFINITIONS )
set(WebP_libwebpmux_COMPILE_DEFINITIONS )
set(WebP_libwebpmux_COMPILE_OPTIONS_C "")
set(WebP_libwebpmux_COMPILE_OPTIONS_CXX "")
set(WebP_libwebpmux_LIBS webpmux)
set(WebP_libwebpmux_SYSTEM_LIBS )
set(WebP_libwebpmux_FRAMEWORK_DIRS )
set(WebP_libwebpmux_FRAMEWORKS )
set(WebP_libwebpmux_BUILD_MODULES_PATHS )
set(WebP_libwebpmux_DEPENDENCIES WebP::webp)
set(WebP_libwebpmux_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)

########### COMPONENT webpdemux VARIABLES #############################################

set(WebP_webpdemux_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webpdemux_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webpdemux_INCLUDES "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webpdemux_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/lib")
set(WebP_webpdemux_RES_DIRS )
set(WebP_webpdemux_DEFINITIONS )
set(WebP_webpdemux_COMPILE_DEFINITIONS )
set(WebP_webpdemux_COMPILE_OPTIONS_C "")
set(WebP_webpdemux_COMPILE_OPTIONS_CXX "")
set(WebP_webpdemux_LIBS webpdemux)
set(WebP_webpdemux_SYSTEM_LIBS )
set(WebP_webpdemux_FRAMEWORK_DIRS )
set(WebP_webpdemux_FRAMEWORKS )
set(WebP_webpdemux_BUILD_MODULES_PATHS )
set(WebP_webpdemux_DEPENDENCIES WebP::webp)
set(WebP_webpdemux_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)

########### COMPONENT webpdecoder VARIABLES #############################################

set(WebP_webpdecoder_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webpdecoder_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webpdecoder_INCLUDES "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include")
set(WebP_webpdecoder_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/libwebp/1.2.2/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/lib")
set(WebP_webpdecoder_RES_DIRS )
set(WebP_webpdecoder_DEFINITIONS )
set(WebP_webpdecoder_COMPILE_DEFINITIONS )
set(WebP_webpdecoder_COMPILE_OPTIONS_C "")
set(WebP_webpdecoder_COMPILE_OPTIONS_CXX "")
set(WebP_webpdecoder_LIBS webpdecoder)
set(WebP_webpdecoder_SYSTEM_LIBS )
set(WebP_webpdecoder_FRAMEWORK_DIRS )
set(WebP_webpdecoder_FRAMEWORKS )
set(WebP_webpdecoder_BUILD_MODULES_PATHS )
set(WebP_webpdecoder_DEPENDENCIES )
set(WebP_webpdecoder_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)


########## FIND PACKAGE DEPENDENCY ##########################################################
#############################################################################################

include(CMakeFindDependencyMacro)


########## FIND LIBRARIES & FRAMEWORKS / DYNAMIC VARS #######################################
#############################################################################################

########## COMPONENT webp FIND LIBRARIES & FRAMEWORKS / DYNAMIC VARS #############

set(WebP_webp_FRAMEWORKS_FOUND "")
conan_find_apple_frameworks(WebP_webp_FRAMEWORKS_FOUND "${WebP_webp_FRAMEWORKS}" "${WebP_webp_FRAMEWORK_DIRS}")

set(WebP_webp_LIB_TARGETS "")
set(WebP_webp_NOT_USED "")
set(WebP_webp_LIBS_FRAMEWORKS_DEPS ${WebP_webp_FRAMEWORKS_FOUND} ${WebP_webp_SYSTEM_LIBS} ${WebP_webp_DEPENDENCIES})
conan_package_library_targets("${WebP_webp_LIBS}"
                              "${WebP_webp_LIB_DIRS}"
                              "${WebP_webp_LIBS_FRAMEWORKS_DEPS}"
                              WebP_webp_NOT_USED
                              WebP_webp_LIB_TARGETS
                              ""
                              "WebP_webp")

set(WebP_webp_LINK_LIBS ${WebP_webp_LIB_TARGETS} ${WebP_webp_LIBS_FRAMEWORKS_DEPS})

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH})

########## COMPONENT libwebpmux FIND LIBRARIES & FRAMEWORKS / DYNAMIC VARS #############

set(WebP_libwebpmux_FRAMEWORKS_FOUND "")
conan_find_apple_frameworks(WebP_libwebpmux_FRAMEWORKS_FOUND "${WebP_libwebpmux_FRAMEWORKS}" "${WebP_libwebpmux_FRAMEWORK_DIRS}")

set(WebP_libwebpmux_LIB_TARGETS "")
set(WebP_libwebpmux_NOT_USED "")
set(WebP_libwebpmux_LIBS_FRAMEWORKS_DEPS ${WebP_libwebpmux_FRAMEWORKS_FOUND} ${WebP_libwebpmux_SYSTEM_LIBS} ${WebP_libwebpmux_DEPENDENCIES})
conan_package_library_targets("${WebP_libwebpmux_LIBS}"
                              "${WebP_libwebpmux_LIB_DIRS}"
                              "${WebP_libwebpmux_LIBS_FRAMEWORKS_DEPS}"
                              WebP_libwebpmux_NOT_USED
                              WebP_libwebpmux_LIB_TARGETS
                              ""
                              "WebP_libwebpmux")

set(WebP_libwebpmux_LINK_LIBS ${WebP_libwebpmux_LIB_TARGETS} ${WebP_libwebpmux_LIBS_FRAMEWORKS_DEPS})

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH})

########## COMPONENT webpdemux FIND LIBRARIES & FRAMEWORKS / DYNAMIC VARS #############

set(WebP_webpdemux_FRAMEWORKS_FOUND "")
conan_find_apple_frameworks(WebP_webpdemux_FRAMEWORKS_FOUND "${WebP_webpdemux_FRAMEWORKS}" "${WebP_webpdemux_FRAMEWORK_DIRS}")

set(WebP_webpdemux_LIB_TARGETS "")
set(WebP_webpdemux_NOT_USED "")
set(WebP_webpdemux_LIBS_FRAMEWORKS_DEPS ${WebP_webpdemux_FRAMEWORKS_FOUND} ${WebP_webpdemux_SYSTEM_LIBS} ${WebP_webpdemux_DEPENDENCIES})
conan_package_library_targets("${WebP_webpdemux_LIBS}"
                              "${WebP_webpdemux_LIB_DIRS}"
                              "${WebP_webpdemux_LIBS_FRAMEWORKS_DEPS}"
                              WebP_webpdemux_NOT_USED
                              WebP_webpdemux_LIB_TARGETS
                              ""
                              "WebP_webpdemux")

set(WebP_webpdemux_LINK_LIBS ${WebP_webpdemux_LIB_TARGETS} ${WebP_webpdemux_LIBS_FRAMEWORKS_DEPS})

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH})

########## COMPONENT webpdecoder FIND LIBRARIES & FRAMEWORKS / DYNAMIC VARS #############

set(WebP_webpdecoder_FRAMEWORKS_FOUND "")
conan_find_apple_frameworks(WebP_webpdecoder_FRAMEWORKS_FOUND "${WebP_webpdecoder_FRAMEWORKS}" "${WebP_webpdecoder_FRAMEWORK_DIRS}")

set(WebP_webpdecoder_LIB_TARGETS "")
set(WebP_webpdecoder_NOT_USED "")
set(WebP_webpdecoder_LIBS_FRAMEWORKS_DEPS ${WebP_webpdecoder_FRAMEWORKS_FOUND} ${WebP_webpdecoder_SYSTEM_LIBS} ${WebP_webpdecoder_DEPENDENCIES})
conan_package_library_targets("${WebP_webpdecoder_LIBS}"
                              "${WebP_webpdecoder_LIB_DIRS}"
                              "${WebP_webpdecoder_LIBS_FRAMEWORKS_DEPS}"
                              WebP_webpdecoder_NOT_USED
                              WebP_webpdecoder_LIB_TARGETS
                              ""
                              "WebP_webpdecoder")

set(WebP_webpdecoder_LINK_LIBS ${WebP_webpdecoder_LIB_TARGETS} ${WebP_webpdecoder_LIBS_FRAMEWORKS_DEPS})

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH})


########## TARGETS ##########################################################################
#############################################################################################

########## COMPONENT webp TARGET #################################################

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    # Target approach
    if(NOT TARGET WebP::webp)
        add_library(WebP::webp INTERFACE IMPORTED)
        set_target_properties(WebP::webp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                              "${WebP_webp_INCLUDE_DIRS}")
        set_target_properties(WebP::webp PROPERTIES INTERFACE_LINK_DIRECTORIES
                              "${WebP_webp_LIB_DIRS}")
        set_target_properties(WebP::webp PROPERTIES INTERFACE_LINK_LIBRARIES
                              "${WebP_webp_LINK_LIBS};${WebP_webp_LINKER_FLAGS_LIST}")
        set_target_properties(WebP::webp PROPERTIES INTERFACE_COMPILE_DEFINITIONS
                              "${WebP_webp_COMPILE_DEFINITIONS}")
        set_target_properties(WebP::webp PROPERTIES INTERFACE_COMPILE_OPTIONS
                              "${WebP_webp_COMPILE_OPTIONS_C};${WebP_webp_COMPILE_OPTIONS_CXX}")
    endif()
endif()

########## COMPONENT libwebpmux TARGET #################################################

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    # Target approach
    if(NOT TARGET WebP::libwebpmux)
        add_library(WebP::libwebpmux INTERFACE IMPORTED)
        set_target_properties(WebP::libwebpmux PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                              "${WebP_libwebpmux_INCLUDE_DIRS}")
        set_target_properties(WebP::libwebpmux PROPERTIES INTERFACE_LINK_DIRECTORIES
                              "${WebP_libwebpmux_LIB_DIRS}")
        set_target_properties(WebP::libwebpmux PROPERTIES INTERFACE_LINK_LIBRARIES
                              "${WebP_libwebpmux_LINK_LIBS};${WebP_libwebpmux_LINKER_FLAGS_LIST}")
        set_target_properties(WebP::libwebpmux PROPERTIES INTERFACE_COMPILE_DEFINITIONS
                              "${WebP_libwebpmux_COMPILE_DEFINITIONS}")
        set_target_properties(WebP::libwebpmux PROPERTIES INTERFACE_COMPILE_OPTIONS
                              "${WebP_libwebpmux_COMPILE_OPTIONS_C};${WebP_libwebpmux_COMPILE_OPTIONS_CXX}")
    endif()
endif()

########## COMPONENT webpdemux TARGET #################################################

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    # Target approach
    if(NOT TARGET WebP::webpdemux)
        add_library(WebP::webpdemux INTERFACE IMPORTED)
        set_target_properties(WebP::webpdemux PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                              "${WebP_webpdemux_INCLUDE_DIRS}")
        set_target_properties(WebP::webpdemux PROPERTIES INTERFACE_LINK_DIRECTORIES
                              "${WebP_webpdemux_LIB_DIRS}")
        set_target_properties(WebP::webpdemux PROPERTIES INTERFACE_LINK_LIBRARIES
                              "${WebP_webpdemux_LINK_LIBS};${WebP_webpdemux_LINKER_FLAGS_LIST}")
        set_target_properties(WebP::webpdemux PROPERTIES INTERFACE_COMPILE_DEFINITIONS
                              "${WebP_webpdemux_COMPILE_DEFINITIONS}")
        set_target_properties(WebP::webpdemux PROPERTIES INTERFACE_COMPILE_OPTIONS
                              "${WebP_webpdemux_COMPILE_OPTIONS_C};${WebP_webpdemux_COMPILE_OPTIONS_CXX}")
    endif()
endif()

########## COMPONENT webpdecoder TARGET #################################################

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    # Target approach
    if(NOT TARGET WebP::webpdecoder)
        add_library(WebP::webpdecoder INTERFACE IMPORTED)
        set_target_properties(WebP::webpdecoder PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                              "${WebP_webpdecoder_INCLUDE_DIRS}")
        set_target_properties(WebP::webpdecoder PROPERTIES INTERFACE_LINK_DIRECTORIES
                              "${WebP_webpdecoder_LIB_DIRS}")
        set_target_properties(WebP::webpdecoder PROPERTIES INTERFACE_LINK_LIBRARIES
                              "${WebP_webpdecoder_LINK_LIBS};${WebP_webpdecoder_LINKER_FLAGS_LIST}")
        set_target_properties(WebP::webpdecoder PROPERTIES INTERFACE_COMPILE_DEFINITIONS
                              "${WebP_webpdecoder_COMPILE_DEFINITIONS}")
        set_target_properties(WebP::webpdecoder PROPERTIES INTERFACE_COMPILE_OPTIONS
                              "${WebP_webpdecoder_COMPILE_OPTIONS_C};${WebP_webpdecoder_COMPILE_OPTIONS_CXX}")
    endif()
endif()

########## GLOBAL TARGET ####################################################################

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    if(NOT TARGET WebP::WebP)
        add_library(WebP::WebP INTERFACE IMPORTED)
    endif()
    set_property(TARGET WebP::WebP APPEND PROPERTY
                 INTERFACE_LINK_LIBRARIES "${WebP_COMPONENTS}")
endif()

########## BUILD MODULES ####################################################################
#############################################################################################
########## COMPONENT webp BUILD MODULES ##########################################

foreach(_BUILD_MODULE_PATH ${WebP_webp_BUILD_MODULES_PATHS})
    include(${_BUILD_MODULE_PATH})
endforeach()
########## COMPONENT libwebpmux BUILD MODULES ##########################################

foreach(_BUILD_MODULE_PATH ${WebP_libwebpmux_BUILD_MODULES_PATHS})
    include(${_BUILD_MODULE_PATH})
endforeach()
########## COMPONENT webpdemux BUILD MODULES ##########################################

foreach(_BUILD_MODULE_PATH ${WebP_webpdemux_BUILD_MODULES_PATHS})
    include(${_BUILD_MODULE_PATH})
endforeach()
########## COMPONENT webpdecoder BUILD MODULES ##########################################

foreach(_BUILD_MODULE_PATH ${WebP_webpdecoder_BUILD_MODULES_PATHS})
    include(${_BUILD_MODULE_PATH})
endforeach()
