cmake_minimum_required(VERSION 3.25)

project(window_resizer
    LANGUAGES CXX
)

set(CXX_STANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# install des libs au bon endroit selon la plateforme
include(GNUInstallDirs)

#gestion options globales de compilation
include( options.cmake )

add_subdirectory(resizer)
