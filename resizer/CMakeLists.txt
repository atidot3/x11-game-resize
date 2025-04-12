cmake_minimum_required(VERSION 3.25)

project(resizer
    VERSION 2.5.1
    LANGUAGES CXX
)

set(EXENAME ${PROJECT_NAME})

add_executable(${EXENAME}
    src/main.cpp
    src/resizer.h
    src/resizer.h
)

target_include_directories(${EXENAME} PRIVATE
    src/
    include/
)

target_include_directories(${EXENAME} SYSTEM INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(${EXENAME}
    PRIVATE
        X11
)

export(TARGETS ${EXENAME} FILE ${EXENAME}Config.cmake)

