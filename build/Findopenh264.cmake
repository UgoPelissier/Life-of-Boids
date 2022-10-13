

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


include(FindPackageHandleStandardArgs)

conan_message(STATUS "Conan: Using autogenerated Findopenh264.cmake")
# Global approach
set(openh264_FOUND 1)
set(openh264_VERSION "2.1.1")

find_package_handle_standard_args(openh264 REQUIRED_VARS
                                  openh264_VERSION VERSION_VAR openh264_VERSION)
mark_as_advanced(openh264_FOUND openh264_VERSION)


set(openh264_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/openh264/2.1.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include")
set(openh264_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/openh264/2.1.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include")
set(openh264_INCLUDES "C:/Users/maxime.naillon/.conan/data/openh264/2.1.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include")
set(openh264_RES_DIRS )
set(openh264_DEFINITIONS )
set(openh264_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)
set(openh264_COMPILE_DEFINITIONS )
set(openh264_COMPILE_OPTIONS_LIST "" "")
set(openh264_COMPILE_OPTIONS_C "")
set(openh264_COMPILE_OPTIONS_CXX "")
set(openh264_LIBRARIES_TARGETS "") # Will be filled later, if CMake 3
set(openh264_LIBRARIES "") # Will be filled later
set(openh264_LIBS "") # Same as openh264_LIBRARIES
set(openh264_SYSTEM_LIBS )
set(openh264_FRAMEWORK_DIRS )
set(openh264_FRAMEWORKS )
set(openh264_FRAMEWORKS_FOUND "") # Will be filled later
set(openh264_BUILD_MODULES_PATHS )

conan_find_apple_frameworks(openh264_FRAMEWORKS_FOUND "${openh264_FRAMEWORKS}" "${openh264_FRAMEWORK_DIRS}")

mark_as_advanced(openh264_INCLUDE_DIRS
                 openh264_INCLUDE_DIR
                 openh264_INCLUDES
                 openh264_DEFINITIONS
                 openh264_LINKER_FLAGS_LIST
                 openh264_COMPILE_DEFINITIONS
                 openh264_COMPILE_OPTIONS_LIST
                 openh264_LIBRARIES
                 openh264_LIBS
                 openh264_LIBRARIES_TARGETS)

# Find the real .lib/.a and add them to openh264_LIBS and openh264_LIBRARY_LIST
set(openh264_LIBRARY_LIST openh264)
set(openh264_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/openh264/2.1.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib")

# Gather all the libraries that should be linked to the targets (do not touch existing variables):
set(_openh264_DEPENDENCIES "${openh264_FRAMEWORKS_FOUND} ${openh264_SYSTEM_LIBS} ")

conan_package_library_targets("${openh264_LIBRARY_LIST}"  # libraries
                              "${openh264_LIB_DIRS}"      # package_libdir
                              "${_openh264_DEPENDENCIES}"  # deps
                              openh264_LIBRARIES            # out_libraries
                              openh264_LIBRARIES_TARGETS    # out_libraries_targets
                              ""                          # build_type
                              "openh264")                                      # package_name

set(openh264_LIBS ${openh264_LIBRARIES})

foreach(_FRAMEWORK ${openh264_FRAMEWORKS_FOUND})
    list(APPEND openh264_LIBRARIES_TARGETS ${_FRAMEWORK})
    list(APPEND openh264_LIBRARIES ${_FRAMEWORK})
endforeach()

foreach(_SYSTEM_LIB ${openh264_SYSTEM_LIBS})
    list(APPEND openh264_LIBRARIES_TARGETS ${_SYSTEM_LIB})
    list(APPEND openh264_LIBRARIES ${_SYSTEM_LIB})
endforeach()

# We need to add our requirements too
set(openh264_LIBRARIES_TARGETS "${openh264_LIBRARIES_TARGETS};")
set(openh264_LIBRARIES "${openh264_LIBRARIES};")

set(CMAKE_MODULE_PATH "C:/Users/maxime.naillon/.conan/data/openh264/2.1.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/" ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH "C:/Users/maxime.naillon/.conan/data/openh264/2.1.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/" ${CMAKE_PREFIX_PATH})

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    # Target approach
    if(NOT TARGET openh264::openh264)
        add_library(openh264::openh264 INTERFACE IMPORTED)
        if(openh264_INCLUDE_DIRS)
            set_target_properties(openh264::openh264 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                  "${openh264_INCLUDE_DIRS}")
        endif()
        set_property(TARGET openh264::openh264 PROPERTY INTERFACE_LINK_LIBRARIES
                     "${openh264_LIBRARIES_TARGETS};${openh264_LINKER_FLAGS_LIST}")
        set_property(TARGET openh264::openh264 PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     ${openh264_COMPILE_DEFINITIONS})
        set_property(TARGET openh264::openh264 PROPERTY INTERFACE_COMPILE_OPTIONS
                     "${openh264_COMPILE_OPTIONS_LIST}")
        
    endif()
endif()

foreach(_BUILD_MODULE_PATH ${openh264_BUILD_MODULES_PATHS})
    include(${_BUILD_MODULE_PATH})
endforeach()
