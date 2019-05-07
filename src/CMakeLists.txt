cmake_minimum_required(VERSION 3.5)

project(app)

add_subdirectory(gui)

file(GLOB sources
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	)
add_executable(${PROJECT_NAME}
    ${sources}
)
target_link_libraries(${PROJECT_NAME}
	gui
)

set(SHADER_FOLDER "\"${CMAKE_CURRENT_SOURCE_DIR}/gui/shaders\"")
target_compile_definitions(${PROJECT_NAME} PRIVATE SHADER_FOLDER=${SHADER_FOLDER})
target_compile_definitions(${PROJECT_NAME} PRIVATE DATA_FOLDER=${DATA_FOLDER})
target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_FONT_FOLDER=${IMGUI_FONT_FOLDER})
