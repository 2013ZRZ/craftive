cmake_minimum_required(VERSION 3.16)

# Qt Creator may pass a package-manager bootstrap script path that does not exist.
# If missing, clear it before `project()` to avoid configure failure.
if(DEFINED CMAKE_PROJECT_INCLUDE_BEFORE AND NOT EXISTS "${CMAKE_PROJECT_INCLUDE_BEFORE}")
    message(WARNING "Ignoring missing CMAKE_PROJECT_INCLUDE_BEFORE: ${CMAKE_PROJECT_INCLUDE_BEFORE}")
    unset(CMAKE_PROJECT_INCLUDE_BEFORE CACHE)
    unset(CMAKE_PROJECT_INCLUDE_BEFORE)
endif()

project(Craftive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_link_options(-fuse-ld=lld-22)

if(WIN32)
    # Reduce generated object/dep file path length for MinGW on deep build dirs.
    set(CMAKE_OBJECT_PATH_MAX 128)
endif()

# ── Auto-discover Qt6 ──────────────────────────────────────────────────────
# Qt6_DIR or CMAKE_PREFIX_PATH can be set externally (env/presets/cmake args).
# If neither provides a valid Qt6, search for it automatically.
#
# Search order:
#   1. $ENV{QT6_DIR} / $ENV{CMAKE_PREFIX_PATH}  (environment variables)
#   2. $ENV{QTDIR}  (legacy Qt env var)
#   3. Registry query (Windows only)
#   4. $HOME/Qt/<version>/<platform>   (macOS / Linux)
#   5. C:\Qt\<version>\<platform>       (Windows default)
#   6. D:\Qt\<version>\<platform> ...   (Windows alternative drives)
if(NOT DEFINED Qt6_DIR AND NOT DEFINED CMAKE_PREFIX_PATH)

    # ── Try env vars ─────────────────────────────────────────────────────
    if(DEFINED ENV{CMAKE_PREFIX_PATH})
        set(CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}" CACHE PATH "" FORCE)
    elseif(DEFINED ENV{QT6_DIR})
        set(CMAKE_PREFIX_PATH "$ENV{QT6_DIR}" CACHE PATH "" FORCE)
    elseif(DEFINED ENV{QTDIR})
        # QTDIR usually points to the version root (e.g. C:\Qt\6.11.0\msvc2022_64)
        set(CMAKE_PREFIX_PATH "$ENV{QTDIR}" CACHE PATH "" FORCE)
    else()

        # ── Platform-specific platform subdirectory name ─────────────────
        if(WIN32)
            # Try msvc2022_64 first, then common alternatives
            set(_qt_platform_names
                "msvc2022_64" "msvc2022_arm64"
                "msvc2019_64" "msvc2019"
                "mingw_64" "mingw"
            )
        elseif(APPLE)
            set(_qt_platform_names "macos" "clang_64")
        else()
            set(_qt_platform_names "gcc_64" "clang_64" "linux_gcc_64")
        endif()

        # ── Search Qt installation roots ─────────────────────────────────
        set(_qt_search_roots "")

        # macOS/Linux: ~/Qt/
        if(DEFINED ENV{HOME})
            list(APPEND _qt_search_roots "$ENV{HOME}/Qt")
        endif()

        # Windows: C:\, D:\, E:\ Qt
        if(WIN32)
            foreach(_drv C D E F G)
                if(EXISTS "${_drv}:/Qt")
                    list(APPEND _qt_search_roots "${_drv}:/Qt")
                endif()
            endforeach()
            # Also check PROGRAMFILES
            if(DEFINED ENV{ProgramFiles})
                list(APPEND _qt_search_roots "$ENV{ProgramFiles}/Qt")
            endif()
        endif()

        # ── Search each root, prefer highest version ────────────────────
        set(_qt_auto_found FALSE)
        foreach(_qt_root ${_qt_search_roots})
            if(NOT EXISTS "${_qt_root}")
                continue()
            endif()
            file(GLOB _qt_versions LIST_DIRECTORIES true "${_qt_root}/*")
            # Sort descending by version so highest is preferred
            list(SORT _qt_versions)
            list(REVERSE _qt_versions)

            foreach(_qt_ver_dir ${_qt_versions})
                if(NOT IS_DIRECTORY "${_qt_ver_dir}")
                    continue()
                endif()
                # Try each platform name under this version
                foreach(_plat ${_qt_platform_names})
                    set(_qt_prefix "${_qt_ver_dir}/${_plat}")
                    if(EXISTS "${_qt_prefix}/lib/cmake/Qt6/Qt6Config.cmake")
                        set(CMAKE_PREFIX_PATH "${_qt_prefix}" CACHE PATH
                            "Auto-discovered Qt6 prefix" FORCE)
                        message(STATUS "Auto-discovered Qt6 at: ${_qt_prefix}")
                        set(_qt_auto_found TRUE)
                        break()
                    endif()
                endforeach()
                if(_qt_auto_found)
                    break()
                endif()
            endforeach()
            if(_qt_auto_found)
                break()
            endif()
        endforeach()

        if(NOT _qt_auto_found)
            message(WARNING "Could not auto-discover Qt6. "
                            "Set CMAKE_PREFIX_PATH or Qt6_DIR manually "
                            "(e.g. -DCMAKE_PREFIX_PATH=C:/Qt/6.11.0/msvc2022_64).")
        endif()

        # Clean up
        unset(_qt_auto_found)
        unset(_qt_versions)
        unset(_qt_ver_dir)
        unset(_qt_prefix)
        unset(_qt_platform_names)
        unset(_qt_search_roots)
        unset(_plat)
    endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS Quick Gui LinguistTools)
qt_standard_project_setup()
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

set(MD3_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/include/md3-qml")

if(NOT EXISTS "${MD3_SOURCE_DIR}/src/Core/CMakeLists.txt")
    message(FATAL_ERROR "Bundled md3 not found at: '${MD3_SOURCE_DIR}'. Recreate the project with scaffold.")
endif()

add_subdirectory("${MD3_SOURCE_DIR}/src/Core" md3Core_build)

file(GLOB_RECURSE APP_QML_FILES 
    CONFIGURE_DEPENDS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "src/frontend/*.qml"
)

file(GLOB_RECURSE APP_CPP_FILES
    CONFIGURE_DEPENDS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "src/*.cpp"
)

file(GLOB_RECURSE APP_TS_FILES
    CONFIGURE_DEPENDS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "locales/craftive_*.ts"
)

qt_add_translations(craftive
   TS_FILES ${APP_TS_FILES}
   QM_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
   RESOURCE_PREFIX "/i18n"
)

set_source_files_properties(
    src/frontend/utils/Typofont.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_executable(craftive
    MANUAL_FINALIZATION
    ${APP_CPP_FILES}
)

qt_add_qml_module(craftive
    URI crt
    VERSION 1.0
    QML_FILES
        ${APP_QML_FILES}
    SOURCES
        src/core/err.hpp src/core/err.cpp
)

qt_add_binary_resources(craftiveRes src/res.qrc)
add_dependencies(craftive craftiveRes)

target_include_directories(craftive
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)
target_compile_definitions(craftive
    PRIVATE
        PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    target_link_libraries(craftive
        PRIVATE
            Qt6::Quick
            md3Core
    )

    set(MD3_WHOLEARCHIVE_OPTIONS /WHOLEARCHIVE:md3Core)
    target_link_options(craftive PRIVATE ${MD3_WHOLEARCHIVE_OPTIONS})
elseif(MINGW OR (UNIX AND NOT APPLE))
    target_link_libraries(craftive
        PRIVATE
            Qt6::Quick
            -Wl,--whole-archive
            md3Core
            -Wl,--no-whole-archive
    )
elseif(APPLE)
    target_link_libraries(craftive
        PRIVATE
            Qt6::Quick
            md3Core
    )
else()
    target_link_libraries(craftive
        PRIVATE
            Qt6::Quick
            md3Core
    )
endif()

qt_import_qml_plugins(craftive)
qt_finalize_executable(craftive)
