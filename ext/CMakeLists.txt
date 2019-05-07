
# glad
add_subdirectory(glad)

# glfw
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_INSTALL OFF CACHE BOOL  "GLFW lib only" )
include(glfw)

# glm
set( GLM_TEST_ENABLE OFF CACHE BOOL "" )
add_subdirectory(glm)
target_include_directories(glm INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glm>)

# imgui
add_subdirectory(imgui)