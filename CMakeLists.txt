#### Fast .stl viewer #####
# Original Project Author: Matt Keeter Copyright 2014-2024
# Author: Paul Tsouchlos Copyright 2017-2024

cmake_minimum_required(VERSION 3.16)

project(fstl LANGUAGES CXX)

# Qt 6 requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(FSTL_CHECK_FORMAT "Check source code formatting with clang-format" OFF)
if(FSTL_CHECK_FORMAT)
    find_program(CLANG_FORMAT_EXE NAMES clang-format)
    if(NOT CLANG_FORMAT_EXE)
        message(FATAL_ERROR "Could not find clang-format executable!")
    endif()

    file(GLOB_RECURSE ALL_SOURCE_FILES CONFIGURE_DEPENDS
        src/*.h src/*.cpp
        qt/*.h qt/*.cpp
        gl/*.h gl/*.cpp
        exe/*.h exe/*.cpp
    )

    add_custom_target(check-format
        COMMAND ${CLANG_FORMAT_EXE}
            --Werror --style=file --dry-run
            ${ALL_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Checking source code formatting"
        VERBATIM)

    add_custom_target(fix-format
        COMMAND ${CLANG_FORMAT_EXE}
            --style=file -i
            ${ALL_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Fixing source code formatting"
        VERBATIM)
endif()

# Set the version number
set(FSTL_VERSION_MAJOR "0")
set(FSTL_VERSION_MINOR "11")
set(FSTL_VERSION_PATCH "1")
set(PROJECT_VERSION "${FSTL_VERSION_MAJOR}.${FSTL_VERSION_MINOR}.${FSTL_VERSION_PATCH}")

message(STATUS "Version: ${PROJECT_VERSION}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Project sources and headers
set(Project_Sources
    src/app.cpp
    src/backdrop.cpp
    src/axis.cpp
    src/canvas.cpp
    src/glmesh.cpp
    src/loader.cpp
    src/main.cpp
    src/mesh.cpp
    src/window.cpp
    src/shaderlightprefs.cpp
)

set(Project_Headers
    src/app.h
    src/backdrop.h
    src/axis.h
    src/canvas.h
    src/glmesh.h
    src/loader.h
    src/mesh.h
    src/window.h
    src/shaderlightprefs.h
)

# Resources
set(Project_Resources qt/qt.qrc gl/gl.qrc)

# OpenGL preference
set(OpenGL_GL_PREFERENCE GLVND)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# Process resources
qt_add_resources(Project_Resources_RCC ${Project_Resources})

# Skip AUTOGEN on generated resource files
set_property(SOURCE ${Project_Resources_RCC} PROPERTY SKIP_AUTOGEN ON)

# Executable (Linux only)
add_executable(fstl
    ${Project_Sources}
    ${Project_Headers}
    ${Project_Resources_RCC}
)

# Linking
target_link_libraries(fstl PRIVATE
    Qt::Widgets
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    OpenGL::GL
    Threads::Threads
)

# Version definition
target_compile_definitions(fstl PRIVATE FSTL_VERSION="${PROJECT_VERSION}")

# Installation
install(TARGETS fstl RUNTIME DESTINATION bin)

# Packaging (Linux only - DEB and RPM)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fast .stl file viewer.")
set(CPACK_PACKAGE_VERSION_MAJOR ${FSTL_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${FSTL_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${FSTL_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")

set(CPACK_GENERATOR "DEB;RPM")

include(CPack)
