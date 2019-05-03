#pragma once

#include <cstdio>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

// inline float get_pixep_ratio() {
// 	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
// 	if(monitor == nullptr) throw "Primary monitor not found.";
// 	float xscale, yscale;
// 	glfwGetMonitorContentScale(monitor, &xscale, &yscale);
// 	return xscale;
// }

GLFWwindow* createGLFWWindow(int width, int height, const char * title, float pixelRatio = 1.f){

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		throw std::runtime_error("Could not init glfw!");

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow *window = glfwCreateWindow(width*pixelRatio, height*pixelRatio, title, nullptr, nullptr);
	if (window == nullptr)
		throw std::runtime_error("Could not create window!");
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Enable vsync (1)

	// Initialize OpenGL loader
	bool err = gl3wInit() != 0;
	if (err)
	{
		throw std::runtime_error("Failed to initialize OpenGL loader!");
	}

	return window;
}

// ~App() {
// 	glfwTerminate();
// }


int main(){
	int width = 720;
	int height = 720;
	GLFWwindow* window = createGLFWWindow(width, height, "Hello GLFW");

	// glfwSetWindowUserPointer(window, this);

	while (!glfwWindowShouldClose(window))
	{

		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// draw
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &width, &height);
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(255, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);


		// draw stuff

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}