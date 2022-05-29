
cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME
    mainmenuLibrary
)
set(LIBRARY_NAME
    mainmenu
)
set(LIBRARY_HEADERS_DIR
    include/${LIBRARY_NAME}
)

# Headers
file(GLOB LIBRARY_HEADERS
    ${LIBRARY_HEADERS_DIR}/*.h
    ${LIBRARY_HEADERS_DIR}/*.hpp
)

set(LIBRARY_SOURCE_DIR
    src
)


# Source
file(GLOB LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/*.c
    ${LIBRARY_SOURCE_DIR}/*.cpp
)

project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCE}
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(mainmenu solution)
target_link_libraries(mainmenu camera)
target_link_libraries(mainmenu imgui)
target_link_libraries(mainmenu window)