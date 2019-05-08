#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include <filesystem.h>
#include <shader_m.h>
#include <camera.h>
#include <camera_drag.h>
#include <mesh.h>
#include <model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void drop_callback(GLFWwindow* window, int count, const char **filenames);
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 1200;

// camera
CameraTracking camera;//(/*glm::vec3(0.0f, 10.f, 30.0f)*/);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(2.f, 1.0f, 2.0f);

glm::vec3 modelTrans(0, 0, 0);
glm::vec3 modelRot(0, 0, 0);

int selectedModel = -1;

std::vector<Model> models;


void Style()
{
	ImGuiStyle & style = ImGui::GetStyle();
	ImVec4 * colors = style.Colors;

	/// 0 = FLAT APPEARENCE
	/// 1 = MORE "3D" LOOK
	int is3D = 0;

	colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border]                 = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark]              = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button]                 = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header]                 = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator]              = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

	style.PopupRounding = 3;

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding  = ImVec2(6, 4);
	style.ItemSpacing   = ImVec2(6, 2);

	style.ScrollbarSize = 18;

	style.WindowBorderSize = 1;
	style.ChildBorderSize  = 1;
	style.PopupBorderSize  = 1;
	style.FrameBorderSize  = is3D;

	style.WindowRounding    = 3;
	style.ChildRounding     = 3;
	style.FrameRounding     = 3;
	style.ScrollbarRounding = 2;
	style.GrabRounding      = 3;

	#ifdef IMGUI_HAS_DOCK
		style.TabBorderSize = is3D;
		style.TabRounding   = 3;

		colors[ImGuiCol_DockingEmptyBg]     = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_Tab]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_TabActive]          = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_TabUnfocused]       = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_DockingPreview]     = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	#endif
}

inline float get_pixep_ratio() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if(monitor == nullptr) throw "Primary monitor not found.";
	float xscale, yscale;
	glfwGetMonitorContentScale(monitor, &xscale, &yscale);
	return xscale;
}

int main()
{
	// glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "example app", NULL, NULL);
	if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

	// app icon
	GLFWimage image;
	image.pixels = stbi_load(DATA_FOLDER"/crl_icon.png", &image.width, &image.height, nullptr, 4);
	glfwSetWindowIcon(window, 1, &image);
	stbi_image_free(image.pixels);

	// tell GLFW to capture our mouse
//	 glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	// Setup Dear ImGui binding
	float pixelRatio = get_pixep_ratio();
	const char* glsl_version = "#version 130";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	Style();
	ImFontConfig cfg;
	cfg.SizePixels = 40 * pixelRatio;
	ImFont *imFont = io.Fonts->AddFontFromFileTTF(IMGUI_FONT_FOLDER"/Roboto-Medium.ttf", 15.0f * pixelRatio, &cfg);
	imFont->DisplayOffset.y = pixelRatio;
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(pixelRatio);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// glfw callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset){
		if(ImGui::GetIO().WantCaptureMouse){
			ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
			return;
		}

		camera.ProcessMouseScroll(yoffset);
	});
	glfwSetDropCallback(window, [](GLFWwindow *window, int count, const char **filenames){
		for (int i = 0; i < count; ++i) {
			models.push_back(Model(filenames[i]));
		}
	});

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program
	Shader lightingShader(SHADER_FOLDER"/2.2.basic_lighting.vs", SHADER_FOLDER"/2.2.basic_lighting.fs");
	Shader lampShader(SHADER_FOLDER"/2.2.lamp.vs", SHADER_FOLDER"/2.2.lamp.fs");

	// load some models
	{
		Model m0(DATA_FOLDER"/nanosuit.obj");
		models.push_back(m0);

		Model m1(DATA_FOLDER"/lego.obj");
		m1.trafo = glm::translate(m1.trafo, {5.f, 0.f, 0.f});
		m1.color = {1.f, 0.5f, 0.31f};
		m1.trafo = glm::scale(m1.trafo, glm::vec3(0.2f));
		models.push_back(m1);

		Model m2(DATA_FOLDER"/robot.obj");
		m2.trafo = glm::translate(m2.trafo, glm::vec3(-10, 0, 0));
		m2.trafo = glm::scale(m2.trafo, glm::vec3(0.03f));
		m2.color = {0.3f, 0.3f, 0.3f};
		models.push_back(m2);
	}

	// render loop
    while (!glfwWindowShouldClose(window))
    {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// draw 3d scene
		{
			// be sure to activate shader when setting uniforms/drawing objects
			lightingShader.use();

			// view/projection transformations
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			glm::mat4 view = camera.GetViewMatrix();
			lightingShader.setMat4("projection", projection);
			lightingShader.setMat4("view", view);

			lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("lightPos", lightPos);
			lightingShader.setVec3("viewPos", camera.position());
			int counterModels = 0;
			for (const auto & m : models) {
				// world transformation
				lightingShader.setMat4("model", m.trafo);
				if(counterModels == selectedModel)
					lightingShader.setVec3("objectColor", {0.5f, 1.5f, 0.31f});
				else
					lightingShader.setVec3("objectColor", m.color);
				m.Draw(lightingShader);
				counterModels++;
			}
		}

		// draw ImGui
		{
			using namespace ImGui;

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();

			ImGui::NewFrame();

			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);
			ImGui::Begin("Hello Imgui");

			int counterModels = 0;
			for( auto &model : models ){
				ImGui::PushID(counterModels);

				// highlight selected model
				if(selectedModel == counterModels){
					ImVec2 p_min = ImGui::GetCursorScreenPos();
					ImVec2 p_max = ImVec2(p_min.x + ImGui::GetContentRegionAvailWidth(), p_min.y + ImGui::GetTextLineHeight());
					ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, IM_COL32(100,100,200,200));
				}

				// list models
				if(ImGui::TreeNode("Model", "Model %d", counterModels)){

					// selected?
					bool isSelected = (selectedModel == counterModels);
					if(ImGui::Checkbox("selected", &isSelected))
						selectedModel = (isSelected) ? counterModels : -1;
					// delete model
					if(ImGui::Button("Delete")){
						models.erase(models.begin() + counterModels);
						TreePop();
						PopID();
						continue;
					}

					ImGui::LabelText("dir", "%s", model.directory.c_str());

					// show all meshes
					if(ImGui::TreeNode("Meshes")){
						ImGui::LabelText("meshes", "%zu", model.meshes.size());
						int counterMeshes = 0;
						for(const auto &mesh : model.meshes){
							ImGui::PushID(counterMeshes);
							if(ImGui::TreeNode("Mesh", "Mesh %d", counterMeshes)){
								ImGui::LabelText("vertices", "%zu", mesh.vertices.size());
								ImGui::LabelText("indices", "%zu", mesh.indices.size());
								ImGui::LabelText("textures", "%zu", mesh.textures.size());

								ImGui::TreePop();
							}
							ImGui::PopID();
							counterMeshes++;
						}

						ImGui::TreePop();
					}

					// show and allow edit of model trafo
					if(ImGui::TreeNode("Transformation")){
						ImGui::PushItemWidth(40);

						if(ImGui::TreeNode("position")){
							ImGui::InputFloat("x", &model.trafo[3][0]);
							ImGui::SameLine();
							ImGui::InputFloat("y", &model.trafo[3][1]);
							ImGui::SameLine();
							ImGui::InputFloat("z", &model.trafo[3][2]);

							ImGui::TreePop();
						}

						if(ImGui::TreeNode("size")){
//							float size = model.trafo[0][0]

							if(ImGui::InputFloat("x", &model.trafo[0][0])){
								model.trafo[1][1] = model.trafo[2][2] = model.trafo[0][0];
							}

							ImGui::TreePop();
						}
						ImGui::PopItemWidth();

						ImGui::TreePop();
					}

					ImGui::TreePop();
				}
				ImGui::PopID();
				counterModels++;
			}

			ImGui::End();
			ImGui::EndFrame();

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}



		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	// Cleanup
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	// glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

	// TODO: put this in the model class
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)){
		if(glfwGetKey(window, GLFW_KEY_L))
			modelRot += glm::vec3(deltaTime, 0, 0);
		if(glfwGetKey(window, GLFW_KEY_J))
			modelRot -= glm::vec3(deltaTime, 0, 0);
		if(glfwGetKey(window, GLFW_KEY_I))
			modelRot += glm::vec3(0, deltaTime, 0);
		if(glfwGetKey(window, GLFW_KEY_K))
			modelRot -= glm::vec3(0, deltaTime, 0);
		if(glfwGetKey(window, GLFW_KEY_U))
			modelRot += glm::vec3(0, 0, deltaTime);
		if(glfwGetKey(window, GLFW_KEY_O))
			modelRot -= glm::vec3(0, 0, deltaTime);
	}
	else {
		if(glfwGetKey(window, GLFW_KEY_L))
			modelTrans += glm::vec3(deltaTime, 0, 0);
		if(glfwGetKey(window, GLFW_KEY_J))
			modelTrans -= glm::vec3(deltaTime, 0, 0);
		if(glfwGetKey(window, GLFW_KEY_I))
			modelTrans += glm::vec3(0, deltaTime, 0);
		if(glfwGetKey(window, GLFW_KEY_K))
			modelTrans -= glm::vec3(0, deltaTime, 0);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
//	if(ImGui::GetIO().WantCaptureMouse)
//		ImGui_ImplGlfw_(window, xoffset, yoffset);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){\
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		camera.ProcessLeftMouseMovement(xoffset, yoffset);
	}
	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS){\
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		camera.ProcessMiddleMouseMovement(xoffset, yoffset);
	}



	lastX = xpos;
	lastY = ypos;
}
