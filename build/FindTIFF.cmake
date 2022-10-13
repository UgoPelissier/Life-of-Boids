

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

conan_message(STATUS "Conan: Using autogenerated FindTIFF.cmake")
# Global approach
set(TIFF_FOUND 1)
set(TIFF_VERSION "4.3.0")

find_package_handle_standard_args(TIFF REQUIRED_VARS
                                  TIFF_VERSION VERSION_VAR TIFF_VERSION)
mark_as_advanced(TIFF_FOUND TIFF_VERSION)


set(TIFF_INCLUDE_DIRS "C:/Users/maxime.naillon/.conan/data/libtiff/4.3.0/_/_/package/a08a8e56550da57f6f3d93bb076a90cc6c19cfa2/include")
set(TIFF_INCLUDE_DIR "C:/Users/maxime.naillon/.conan/data/libtiff/4.3.0/_/_/package/a08a8e56550da57f6f3d93bb076a90cc6c19cfa2/include")
set(TIFF_INCLUDES "C:/Users/maxime.naillon/.conan/data/libtiff/4.3.0/_/_/package/a08a8e56550da57f6f3d93bb076a90cc6c19cfa2/include")
set(TIFF_RES_DIRS )
set(TIFF_DEFINITIONS )
set(TIFF_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)
set(TIFF_COMPILE_DEFINITIONS )
set(TIFF_COMPILE_OPTIONS_LIST "" "")
set(TIFF_COMPILE_OPTIONS_C "")
set(TIFF_COMPILE_OPTIONS_CXX "")
set(TIFF_LIBRARIES_TARGETS "") # Will be filled later, if CMake 3
set(TIFF_LIBRARIES "") # Will be filled later
set(TIFF_LIBS "") # Same as TIFF_LIBRARIES
set(TIFF_SYSTEM_LIBS )
set(TIFF_FRAMEWORK_DIRS )
set(TIFF_FRAMEWORKS )
set(TIFF_FRAMEWORKS_FOUND "") # Will be filled later
set(TIFF_BUILD_MODULES_PATHS )

conan_find_apple_frameworks(TIFF_FRAMEWORKS_FOUND "${TIFF_FRAMEWORKS}" "${TIFF_FRAMEWORK_DIRS}")

mark_as_advanced(TIFF_INCLUDE_DIRS
                 TIFF_INCLUDE_DIR
                 TIFF_INCLUDES
                 TIFF_DEFINITIONS
                 TIFF_LINKER_FLAGS_LIST
                 TIFF_COMPILE_DEFINITIONS
                 TIFF_COMPILE_OPTIONS_LIST
                 TIFF_LIBRARIES
                 TIFF_LIBS
                 TIFF_LIBRARIES_TARGETS)

# Find the real .lib/.a and add them to TIFF_LIBS and TIFF_LIBRARY_LIST
set(TIFF_LIBRARY_LIST tiffxx tiff)
set(TIFF_LIB_DIRS "C:/Users/maxime.naillon/.conan/data/libtiff/4.3.0/_/_/package/a08a8e56550da57f6f3d93bb076a90cc6c19cfa2/lib")

# Gather all the libraries that should be linked to the targets (do not touch existing variables):
set(_TIFF_DEPENDENCIES "${TIFF_FRAMEWORKS_FOUND} ${TIFF_SYSTEM_LIBS} ZLIB::ZLIB;libdeflate::libdeflate;LibLZMA::LibLZMA;JPEG::JPEG;jbig::jbig;zstd::zstd;WebP::WebP")

conan_package_library_targets("${TIFF_LIBRARY_LIST}"  # libraries
                              "${TIFF_LIB_DIRS}"      # package_libdir
                              "${_TIFF_DEPENDENCIES}"  # deps
                              TIFF_LIBRARIES            # out_libraries
                              TIFF_LIBRARIES_TARGETS    # out_libraries_targets
                              ""                          # build_type
                              "TIFF")                                      # package_name

set(TIFF_LIBS ${TIFF_LIBRARIES})

foreach(_FRAMEWORK ${TIFF_FRAMEWORKS_FOUND})
    list(APPEND TIFF_LIBRARIES_TARGETS ${_FRAMEWORK})
    list(APPEND TIFF_LIBRARIES ${_FRAMEWORK})
endforeach()

foreach(_SYSTEM_LIB ${TIFF_SYSTEM_LIBS})
    list(APPEND TIFF_LIBRARIES_TARGETS ${_SYSTEM_LIB})
    list(APPEND TIFF_LIBRARIES ${_SYSTEM_LIB})
endforeach()

# We need to add our requirements too
set(TIFF_LIBRARIES_TARGETS "${TIFF_LIBRARIES_TARGETS};ZLIB::ZLIB;libdeflate::libdeflate;LibLZMA::LibLZMA;JPEG::JPEG;jbig::jbig;zstd::zstd;WebP::WebP")
set(TIFF_LIBRARIES "${TIFF_LIBRARIES};ZLIB::ZLIB;libdeflate::libdeflate;LibLZMA::LibLZMA;JPEG::JPEG;jbig::jbig;zstd::zstd;WebP::WebP")

set(CMAKE_MODULE_PATH "C:/Users/maxime.naillon/.conan/data/libtiff/4.3.0/_/_/package/a08a8e56550da57f6f3d93bb076a90cc6c19cfa2/" ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH "C:/Users/maxime.naillon/.conan/data/libtiff/4.3.0/_/_/package/a08a8e56550da57f6f3d93bb076a90cc6c19cfa2/" ${CMAKE_PREFIX_PATH})

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    # Target approach
    if(NOT TARGET TIFF::TIFF)
        add_library(TIFF::TIFF INTERFACE IMPORTED)
        if(TIFF_INCLUDE_DIRS)
            set_target_properties(TIFF::TIFF PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                  "${TIFF_INCLUDE_DIRS}")
        endif()
        set_property(TARGET TIFF::TIFF PROPERTY INTERFACE_LINK_LIBRARIES
                     "${TIFF_LIBRARIES_TARGETS};${TIFF_LINKER_FLAGS_LIST}")
        set_property(TARGET TIFF::TIFF PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     ${TIFF_COMPILE_DEFINITIONS})
        set_property(TARGET TIFF::TIFF PROPERTY INTERFACE_COMPILE_OPTIONS
                     "${TIFF_COMPILE_OPTIONS_LIST}")
        
        # Library dependencies
        include(CMakeFindDependencyMacro)

        if(NOT ZLIB_FOUND)
            find_dependency(ZLIB REQUIRED)
        else()
            message(STATUS "Dependency ZLIB already found")
        endif()


        if(NOT libdeflate_FOUND)
            find_dependency(libdeflate REQUIRED)
        else()
            message(STATUS "Dependency libdeflate already found")
        endif()


        if(NOT LibLZMA_FOUND)
            find_dependency(LibLZMA REQUIRED)
        else()
            message(STATUS "Dependency LibLZMA already found")
        endif()


        if(NOT JPEG_FOUND)
            find_dependency(JPEG REQUIRED)
        else()
            message(STATUS "Dependency JPEG already found")
        endif()


        if(NOT jbig_FOUND)
            find_dependency(jbig REQUIRED)
        else()
            message(STATUS "Dependency jbig already found")
        endif()


        if(NOT zstd_FOUND)
            find_dependency(zstd REQUIRED)
        else()
            message(STATUS "Dependency zstd already found")
        endif()


        if(NOT WebP_FOUND)
            find_dependency(WebP REQUIRED)
        else()
            message(STATUS "Dependency WebP already found")
        endif()

    endif()
endif()

foreach(_BUILD_MODULE_PATH ${TIFF_BUILD_MODULES_PATHS})
    include(${_BUILD_MODULE_PATH})
endforeach()
