#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

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

	Model rockModel(DATA_FOLDER"/nanosuit.obj");

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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, modelRot[0], glm::vec3(1, 0, 0));
		model = glm::rotate(model, modelRot[1], glm::vec3(0, 1, 0));
		model = glm::rotate(model, modelRot[2], glm::vec3(0, 0, 1));
		model = glm::translate(model, modelTrans);
        lightingShader.setMat4("model", model);

		rockModel.Draw(lightingShader);
        // render the cube
        glBindVertexArray(cubeVAO);
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
