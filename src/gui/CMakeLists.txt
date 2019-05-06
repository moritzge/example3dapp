cmake_minimum_required(VERSION 3.5)

project(gui)

find_package(OpenGL REQUIRED)

file(GLOB sources
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vs
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.fs
)

add_library(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME}
	glad
	${OPENGL_LIBRARIES}
    glfw
	glm
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
