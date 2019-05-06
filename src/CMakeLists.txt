cmake_minimum_required(VERSION 3.5)

add_subdirectory(gui)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(app
    ${sources}
)
target_link_libraries(app
 	gui
)

set(SHADER_FOLDER "\"${CMAKE_CURRENT_SOURCE_DIR}/gui/shaders\"")
target_compile_definitions(app PRIVATE SHADER_FOLDER=${SHADER_FOLDER})
