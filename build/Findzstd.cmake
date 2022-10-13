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

conan_message(STATUS "Conan: Using autogenerated Findzstd.cmake")
set(zstd_FOUND 1)
set(zstd_VERSION "1.5.2")

find_package_handle_standard_args(zstd REQUIRED_VARS
                                  zstd_VERSION VERSION_VAR zstd_VERSION)
mark_as_advanced(zstd_FOUND zstd_VERSION)

set(zstd_COMPONENTS zstd::libzstd_static)

if(zstd_FIND_COMPONENTS)
    foreach(_FIND_COMPONENT ${zstd_FIND_COMPONENTS})
        list(FIND zstd_COMPONENTS "zstd::${_FIND_COMPONENT}" _index)
        if(${_index} EQUAL -1)
            conan_message(FATAL_ERROR "Conan: Component '${_FIND_COMPONENT}' NOT found in package 'zstd'")
        else()
            conan_message(STATUS "Conan: Component '${_FIND_COMPONENT}' found in package 'zstd'")
        endif()
    endforeach()
endif()

########### VARIABLES #######################################################################
#############################################################################################


set(zstd_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/include")
set(zstd_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/include")
set(zstd_INCLUDES "C:/Users/maxime.naillon/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/include")
set(zstd_RES_DIRS )
set(zstd_DEFINITIONS )
set(zstd_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)
set(zstd_COMPILE_DEFINITIONS )
set(zstd_COMPILE_OPTIONS_LIST "" "")
set(zstd_COMPILE_OPTIONS_C "")
set(zstd_COMPILE_OPTIONS_CXX "")
set(zstd_LIBRARIES_TARGETS "") # Will be filled later, if CMake 3
set(zstd_LIBRARIES "") # Will be filled later
set(zstd_LIBS "") # Same as zstd_LIBRARIES
set(zstd_SYSTEM_LIBS )
set(zstd_FRAMEWORK_DIRS )
set(zstd_FRAMEWORKS )
set(zstd_FRAMEWORKS_FOUND "") # Will be filled later
set(zstd_BUILD_MODULES_PATHS )

conan_find_apple_frameworks(zstd_FRAMEWORKS_FOUND "${zstd_FRAMEWORKS}" "${zstd_FRAMEWORK_DIRS}")

mark_as_advanced(zstd_INCLUDE_DIRS
                 zstd_INCLUDE_DIR
                 zstd_INCLUDES
                 zstd_DEFINITIONS
                 zstd_LINKER_FLAGS_LIST
                 zstd_COMPILE_DEFINITIONS
                 zstd_COMPILE_OPTIONS_LIST
                 zstd_LIBRARIES
                 zstd_LIBS
                 zstd_LIBRARIES_TARGETS)

# Find the real .lib/.a and add them to zstd_LIBS and zstd_LIBRARY_LIST
set(zstd_LIBRARY_LIST zstd_static)
set(zstd_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/lib")

# Gather all the libraries that should be linked to the targets (do not touch existing variables):
set(_zstd_DEPENDENCIES "${zstd_FRAMEWORKS_FOUND} ${zstd_SYSTEM_LIBS} ")

conan_package_library_targets("${zstd_LIBRARY_LIST}"  # libraries
                              "${zstd_LIB_DIRS}"      # package_libdir
                              "${_zstd_DEPENDENCIES}"  # deps
                              zstd_LIBRARIES            # out_libraries
                              zstd_LIBRARIES_TARGETS    # out_libraries_targets
                              ""                          # build_type
                              "zstd")                                      # package_name

set(zstd_LIBS ${zstd_LIBRARIES})

foreach(_FRAMEWORK ${zstd_FRAMEWORKS_FOUND})
    list(APPEND zstd_LIBRARIES_TARGETS ${_FRAMEWORK})
    list(APPEND zstd_LIBRARIES ${_FRAMEWORK})
endforeach()

foreach(_SYSTEM_LIB ${zstd_SYSTEM_LIBS})
    list(APPEND zstd_LIBRARIES_TARGETS ${_SYSTEM_LIB})
    list(APPEND zstd_LIBRARIES ${_SYSTEM_LIB})
endforeach()

# We need to add our requirements too
set(zstd_LIBRARIES_TARGETS "${zstd_LIBRARIES_TARGETS};")
set(zstd_LIBRARIES "${zstd_LIBRARIES};")

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH})


########### COMPONENT libzstd_static VARIABLES #############################################

set(zstd_libzstd_static_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/include")
set(zstd_libzstd_static_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/include")
set(zstd_libzstd_static_INCLUDES "C:/Users/maxime.naillon/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/include")
set(zstd_libzstd_static_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/lib")
set(zstd_libzstd_static_RES_DIRS )
set(zstd_libzstd_static_DEFINITIONS )
set(zstd_libzstd_static_COMPILE_DEFINITIONS )
set(zstd_libzstd_static_COMPILE_OPTIONS_C "")
set(zstd_libzstd_static_COMPILE_OPTIONS_CXX "")
set(zstd_libzstd_static_LIBS zstd_static)
set(zstd_libzstd_static_SYSTEM_LIBS )
set(zstd_libzstd_static_FRAMEWORK_DIRS )
set(zstd_libzstd_static_FRAMEWORKS )
set(zstd_libzstd_static_BUILD_MODULES_PATHS )
set(zstd_libzstd_static_DEPENDENCIES )
set(zstd_libzstd_static_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)


########## FIND PACKAGE DEPENDENCY ##########################################################
#############################################################################################

include(CMakeFindDependencyMacro)


########## FIND LIBRARIES & FRAMEWORKS / DYNAMIC VARS #######################################
#############################################################################################

########## COMPONENT libzstd_static FIND LIBRARIES & FRAMEWORKS / DYNAMIC VARS #############

set(zstd_libzstd_static_FRAMEWORKS_FOUND "")
conan_find_apple_frameworks(zstd_libzstd_static_FRAMEWORKS_FOUND "${zstd_libzstd_static_FRAMEWORKS}" "${zstd_libzstd_static_FRAMEWORK_DIRS}")

set(zstd_libzstd_static_LIB_TARGETS "")
set(zstd_libzstd_static_NOT_USED "")
set(zstd_libzstd_static_LIBS_FRAMEWORKS_DEPS ${zstd_libzstd_static_FRAMEWORKS_FOUND} ${zstd_libzstd_static_SYSTEM_LIBS} ${zstd_libzstd_static_DEPENDENCIES})
conan_package_library_targets("${zstd_libzstd_static_LIBS}"
                              "${zstd_libzstd_static_LIB_DIRS}"
                              "${zstd_libzstd_static_LIBS_FRAMEWORKS_DEPS}"
                              zstd_libzstd_static_NOT_USED
                              zstd_libzstd_static_LIB_TARGETS
                              ""
                              "zstd_libzstd_static")

set(zstd_libzstd_static_LINK_LIBS ${zstd_libzstd_static_LIB_TARGETS} ${zstd_libzstd_static_LIBS_FRAMEWORKS_DEPS})

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH})


########## TARGETS ##########################################################################
#############################################################################################

########## COMPONENT libzstd_static TARGET #################################################

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    # Target approach
    if(NOT TARGET zstd::libzstd_static)
        add_library(zstd::libzstd_static INTERFACE IMPORTED)
        set_target_properties(zstd::libzstd_static PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                              "${zstd_libzstd_static_INCLUDE_DIRS}")
        set_target_properties(zstd::libzstd_static PROPERTIES INTERFACE_LINK_DIRECTORIES
                              "${zstd_libzstd_static_LIB_DIRS}")
        set_target_properties(zstd::libzstd_static PROPERTIES INTERFACE_LINK_LIBRARIES
                              "${zstd_libzstd_static_LINK_LIBS};${zstd_libzstd_static_LINKER_FLAGS_LIST}")
        set_target_properties(zstd::libzstd_static PROPERTIES INTERFACE_COMPILE_DEFINITIONS
                              "${zstd_libzstd_static_COMPILE_DEFINITIONS}")
        set_target_properties(zstd::libzstd_static PROPERTIES INTERFACE_COMPILE_OPTIONS
                              "${zstd_libzstd_static_COMPILE_OPTIONS_C};${zstd_libzstd_static_COMPILE_OPTIONS_CXX}")
    endif()
endif()

########## GLOBAL TARGET ####################################################################

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    if(NOT TARGET zstd::zstd)
        add_library(zstd::zstd INTERFACE IMPORTED)
    endif()
    set_property(TARGET zstd::zstd APPEND PROPERTY
                 INTERFACE_LINK_LIBRARIES "${zstd_COMPONENTS}")
endif()

########## BUILD MODULES ####################################################################
#############################################################################################
########## COMPONENT libzstd_static BUILD MODULES ##########################################

foreach(_BUILD_MODULE_PATH ${zstd_libzstd_static_BUILD_MODULES_PATHS})
    include(${_BUILD_MODULE_PATH})
endforeach()
