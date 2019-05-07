cmake_minimum_required(VERSION 3.5)

project(gui)

find_package(OpenGL REQUIRED)

find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

file(GLOB sources
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vs
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.fs
	${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl/*.cpp
)

add_library(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME}
	glad
	${OPENGL_LIBRARIES}
    glfw
	glm
	assimp
	imgui
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
