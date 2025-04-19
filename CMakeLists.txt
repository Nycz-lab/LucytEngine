cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

project(LucytEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set (Python3_FIND_VIRTUALENV FIRST)

option(LUCYT_VENDORED "Use vendored libraries (where possible)" ON)
option(LUCYT_STATIC "link libraries statically" OFF)


if(LUCYT_STATIC)
    set(SDL_STATIC ON)
endif()

if(LUCYT_VENDORED)
    add_subdirectory(third-party/SDL EXCLUDE_FROM_ALL)
    add_subdirectory(third-party/glad/cmake EXCLUDE_FROM_ALL)
    glad_add_library(glad_gl_core_33 SHARED API gl:core=3.3)
    message("using vendored libraries")
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    find_package(GLAD REQUIRED)
    message("using system libraries")
endif()


# add_subdirectory(src)
# add_subdirectory(include)

add_executable( LucytEngine ${PROJECT_SOURCE_DIR}/src/main.cpp )

if(LUCYT_STATIC)
    target_link_libraries(LucytEngine PUBLIC SDL3::SDL3-static glad_gl_core_33)
else()
    target_link_libraries(LucytEngine PUBLIC SDL3::SDL3-shared glad_gl_core_33)
endif()


# This creates a lucy_version.h file which contains the current git tag + revision
find_package(Git)
add_custom_target(version
  ${CMAKE_COMMAND} -D SRC=${PROJECT_SOURCE_DIR}/src/lucyt_version.h.in
                   -D DST=${PROJECT_SOURCE_DIR}/src/lucyt_version.h
                   -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                   -P ${CMAKE_SOURCE_DIR}/GenerateVersionHeader.cmake
  )
add_dependencies(LucytEngine version)

