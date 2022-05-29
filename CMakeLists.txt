
cmake_minimum_required(VERSION 3.0.0)

set(LORENZ_PROJECT_NAME
    Lorenz
)
set(LORENZ_SOURCE_DIR
    src
)
set(LORENZ_SOURCE
    ${LORENZ_SOURCE_DIR}/main.cpp
)
set(LORENZ_LIBRARIES_DIR
    libs
)
set(LORENZ_LIBRARIES 
    glad
    glfw
    imgui
    glm
    quaternion
       
)

set(LORENZ_PROJECT_LIBRARIES_DIR
    project-libs
)
set(LORENZ_PROJECT_LIBRARIES 
    mainmenu
    shader
    window
    camera
    solution    
)

project(${LORENZ_PROJECT_NAME})


###########
# Libs #
###########

add_executable(${LORENZ_PROJECT_NAME} ${LORENZ_SOURCE})

find_package(SDL2 REQUIRED)

foreach(LIBRARY ${LORENZ_LIBRARIES})
    add_subdirectory("${LORENZ_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)

target_link_libraries(${LORENZ_PROJECT_NAME} ${LORENZ_LIBRARIES})

foreach(LIBRARY ${LORENZ_PROJECT_LIBRARIES})
    add_subdirectory("${LORENZ_PROJECT_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)

target_link_libraries(${LORENZ_PROJECT_NAME} ${LORENZ_PROJECT_LIBRARIES})

