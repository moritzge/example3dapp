cmake_minimum_required(VERSION 3.5)

PROJECT(gui)

find_package(OpenGL REQUIRED)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(gui ${sources})
set_target_properties(gui PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(gui
    ${OPENGL_LIBRARIES} # open gl libs already linked by gl3w, linking again ...
	glad
    glfw
    glm
)
target_include_directories(gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/glfw/include
)
