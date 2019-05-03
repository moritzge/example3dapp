cmake_minimum_required(VERSION 3.5)

find_package(OpenGL REQUIRED)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(app
    ${sources}
)
target_link_libraries(app
    ${OPENGL_LIBRARIES} # open gl libs already linked by gl3w, linking again ...
    gl3w
    glfw
)
target_include_directories(app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/glfw/include
)