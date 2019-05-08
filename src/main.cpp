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
Camera camera(glm::vec3(0.0f, 10.f, 30.0f));
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
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "example app", NULL, NULL);
	if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

	// glfw callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

	GLFWimage image;
	image.pixels = stbi_load(DATA_FOLDER"/crl_icon.png", &image.width, &image.height, nullptr, 4);
	glfwSetWindowIcon(window, 1, &image);
	stbi_image_free(image.pixels);

	// tell GLFW to capture our mouse
//	 glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
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

//	ImGui::GetIO().Fonts->AddFont(&cfg);// (imFont->ConfigData)->DisplayOffset.y = pixelRatio;
//	imFont->ConfigData->SizePixels = 13 * pixelRatio;
	// Setup style
//	ImGui::StyleColorsDark();
//	ImGui::StyleColorsLight();

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
	Shader lightingShader(SHADER_FOLDER"/2.2.basic_lighting.vs", SHADER_FOLDER"/2.2.basic_lighting.fs");
	Shader lampShader(SHADER_FOLDER"/2.2.lamp.vs", SHADER_FOLDER"/2.2.lamp.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	Model m0(DATA_FOLDER"/nanosuit.obj");
	models.push_back(m0);
	Model m1(DATA_FOLDER"/nanosuit.obj");
	m1.trafo = glm::translate(m1.trafo, {10.f, 0.f, 0.f});
	m1.color = {1.f, 0.5f, 0.31f};
	models.push_back(m1);

	glfwSetDropCallback(window, [](GLFWwindow *window, int count, const char **filenames){
		for (int i = 0; i < count; ++i) {
			models.push_back(Model(filenames[i]));
		}
	});

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);

		{
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

        // render the cube
//        glBindVertexArray(cubeVAO);
//		glDrawArrays(GL_TRIANGLES, 0, 36);

//        // also draw the lamp object
//        lampShader.use();
//        lampShader.setMat4("projection", projection);
//		lampShader.setMat4("view", view);
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, lightPos);
//		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
//        lampShader.setMat4("model", model);

//        glBindVertexArray(lightVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);
		ImGui::Begin("Hello Imgui");
		ImGui::Button("Hi!");

		int counterModels = 0;
		for( auto &model : models ){
			ImGui::PushID(counterModels);
			if(selectedModel == counterModels){
				ImVec2 p_min = ImGui::GetCursorScreenPos();
				ImVec2 p_max = ImVec2(p_min.x + ImGui::GetContentRegionAvailWidth(), p_min.y + ImGui::GetTextLineHeight());
				ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, IM_COL32(100,100,200,200));
			}
			if(ImGui::TreeNode("Model", "Model %d", counterModels)){
				bool isSelected = (selectedModel == counterModels);
				if(ImGui::Checkbox("selected", &isSelected))
					selectedModel = (isSelected) ? counterModels : -1;
				if(ImGui::Button("Delete")){
					models.erase(models.begin() + counterModels);
					continue;
				}

				ImGui::LabelText("dir", "%s", model.directory.c_str());

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
				if(ImGui::TreeNode("Transformation")){
					ImGui::PushItemWidth(40);
					ImGui::InputFloat("x", &model.trafo[3][0]);
					ImGui::SameLine();
					ImGui::InputFloat("y", &model.trafo[3][1]);
					ImGui::SameLine();
					ImGui::InputFloat("z", &model.trafo[3][2]);
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



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

	// Cleanup
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(ImGui::GetIO().WantCaptureMouse)
		return;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){\
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
//	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS){\
//		float xoffset = xpos - lastX;
//		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//		camera.ProcessMiddleMouseMovement(xoffset, yoffset);
//	}



	lastX = xpos;
	lastY = ypos;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <iostream>

// static void glfw_error_callback(int error, const char* description)
// {
// 	fprintf(stderr, "Error %d: %s\n", error, description);
// }

// GLFWwindow* createGLFWWindow(int width, int height, const char * title, float pixelRatio = 1.f){

// 	// Setup window
// 	glfwSetErrorCallback(glfw_error_callback);
// 	if (!glfwInit())
// 		throw std::runtime_error("Could not init glfw!");

// 	// Decide GL+GLSL versions
// #if __APPLE__
// 	// GL 3.2 + GLSL 150
// 	const char* glsl_version = "#version 150";
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
// 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
// #else
// 	// GL 3.0 + GLSL 130
// 	const char* glsl_version = "#version 130";
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
// 	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
// 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
// #endif

// 	// Create window with graphics context
// 	GLFWwindow *window = glfwCreateWindow(width*pixelRatio, height*pixelRatio, title, nullptr, nullptr);
// 	if (window == nullptr)
// 		throw std::runtime_error("Could not create window!");
// 	glfwMakeContextCurrent(window);
// 	glfwSwapInterval(0); // Enable vsync (1)

// 	// glad: load all OpenGL function pointers
// 	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
// 		throw std::runtime_error("Failed to initialize GLAD.");

// 	return window;
// }

// static const char *vertexShaderSource = "#version 330 core\n"
// 	"layout (location = 0) in vec3 aPos;\n"
// 	"void main()\n"
// 	"{\n"
// 	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
// 	"}\0";
// static const char *fragmentShaderSource = "#version 330 core\n"
// 	"out vec4 FragColor;\n"
// 	"void main()\n"
// 	"{\n"
// 	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
// 	"}\n\0";

// int main(){
// 	int width = 720;
// 	int height = 720;
// 	GLFWwindow* window = createGLFWWindow(width, height, "Hello GLFW");

// 	// build and compile our shader program
// 	// ------------------------------------
// 	// vertex shader
// 	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
// 	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
// 	glCompileShader(vertexShader);
// 	// check for shader compile errors
// 	int success;
// 	char infoLog[512];
// 	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
// 	if (!success)
// 	{
// 		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
// 		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
// 	}
// 	// fragment shader
// 	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
// 	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
// 	glCompileShader(fragmentShader);
// 	// check for shader compile errors
// 	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
// 	if (!success)
// 	{
// 		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
// 		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
// 	}
// 	// link shaders
// 	int shaderProgram = glCreateProgram();
// 	glAttachShader(shaderProgram, vertexShader);
// 	glAttachShader(shaderProgram, fragmentShader);
// 	glLinkProgram(shaderProgram);
// 	// check for linking errors
// 	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
// 	if (!success) {
// 		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
// 		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
// 	}
// 	glDeleteShader(vertexShader);
// 	glDeleteShader(fragmentShader);

// 	// set up vertex data (and buffer(s)) and configure vertex attributes
// 	// ------------------------------------------------------------------
// 	float vertices[] = {
// 		 0.5f,  0.5f, 0.0f,  // top right
// 		 0.5f, -0.5f, 0.0f,  // bottom right
// 		-0.5f, -0.5f, 0.0f,  // bottom left
// 		-0.5f,  0.5f, 0.0f   // top left
// 	};
// 	unsigned int indices[] = {  // note that we start from 0!
// 		0, 1, 3,  // first Triangle
// 		1, 2, 3   // second Triangle
// 	};
// 	unsigned int VBO, VAO, EBO;
// 	glGenVertexArrays(1, &VAO);
// 	glGenBuffers(1, &VBO);
// 	glGenBuffers(1, &EBO);
// 	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
// 	glBindVertexArray(VAO);

// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);

// 	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

// 	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
// 	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

// 	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// 	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
// //	glBindVertexArray(0);


// 	// uncomment this call to draw in wireframe polygons.
// 	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// 	while (!glfwWindowShouldClose(window))
// 	{
// 		glfwPollEvents();

// 		// draw
// 		glfwMakeContextCurrent(window);
// 		glfwGetFramebufferSize(window, &width, &height);
// 		glfwGetWindowSize(window, &width, &height);
// 		glViewport(0, 0, width, height);
// 		glClearColor(255, 0, 0, 0);
// 		glClear(GL_COLOR_BUFFER_BIT);

// 		// render
// 		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT);

// 		glUseProgram(shaderProgram);
// //		glBindVertexArray(VAO);
// 		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 		// glBindVertexArray(0); // no need to unbind it every time

// 		glfwMakeContextCurrent(window);
// 		glfwSwapBuffers(window);
// 	}

// 	glDeleteVertexArrays(1, &VAO);
// 	glDeleteBuffers(1, &VBO);
// 	glDeleteBuffers(1, &EBO);

// 	glfwTerminate();
// }
