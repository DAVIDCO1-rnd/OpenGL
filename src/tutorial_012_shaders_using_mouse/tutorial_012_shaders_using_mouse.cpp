//https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/3.3.shaders_class/shaders_class.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/shader_s.h"
#include "stb_image.h"

#include <iostream>
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLfloat iMouseX = 0.0f;
GLfloat iMouseY = 0.0f;
GLfloat iMouseZ = 0.0f;
GLfloat iMouseW = 0.0f;

unsigned int textureID;


/*Initializing texture parametrs and loading the texture to openGL*/
/*parameters:
textureData - the pixels of the texture
textureWidth - width of texture
textureHeight - height of texture
textureNumOfChannels - num of channels (bytes) for each pixel
*/
void initializeTexture(unsigned char *textureData, int textureWidth, int textureHeight, int textureNumOfChannels)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format;
	if (textureNumOfChannels == 1)
		format = GL_RED;
	else if (textureNumOfChannels == 3)
		format = GL_RGB;
	else if (textureNumOfChannels == 4)
		format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, textureData);
}

void initializeTextureAndLoadImage(char* filePath)
{
	// load and create a texture 
	int textureWidth, textureHeight, textureNumOfChannels;
	unsigned char *textureData = stbi_load(filePath, &textureWidth, &textureHeight, &textureNumOfChannels, 0);
	if (textureData)
	{
		initializeTexture(textureData, textureWidth, textureHeight, textureNumOfChannels);
		stbi_image_free(textureData);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}

//static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT) {
//		if (GLFW_PRESS == action)
//			lbutton_down = true;
//		else if (GLFW_RELEASE == action)
//			lbutton_down = false;
//	}
//
//	if (lbutton_down) {
//		// do your drag here
//	}
//}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	/*
	in shadertoy:

	vec4 mouse = iMouse;

	mouse.xy = mouse position during last button down
	abs(mouse.zw) = mouse position during last button click
	sign(mouze.z) = button is down (positive if down)
	sign(mouze.w) = button is clicked (positive if clicked)
	*/

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double xposDouble, yposDouble;
		//getting cursor position
		glfwGetCursorPos(window, &xposDouble, &yposDouble);
		GLfloat xpos = (GLfloat)xposDouble;
		GLfloat yUpsideDown = (GLfloat)SCR_HEIGHT - (GLfloat)yposDouble;

		if (action == GLFW_PRESS)
		{
			//button is now down (until released)
			iMouseX = xpos;
			iMouseY = yUpsideDown;

			iMouseZ = xpos;
			iMouseW = yUpsideDown;
		}

		if (action == GLFW_RELEASE)
		{
			//button is not down anymore
			iMouseZ = -xpos;
			iMouseW = -yUpsideDown;
		}
		
		
		//std::cout << "Cursor Position at (" << xpos << " : " << ypos << ")" << std::endl;
	}

	//if (button == GLFW_MOUSE_BUTTON_LAST)
	////if (button == GLFW_MOUSE_BUTTON_LAST && action == GLFW_PRESS)
	//{
	//	//button is clicked (not down)
	//	double xpos, ypos;
	//	//getting cursor position
	//	glfwGetCursorPos(window, &xpos, &ypos);
	//	iMouseX = (GLfloat)xpos; //positive since it's down
	//	GLfloat yUpsideDown = (GLfloat)SCR_HEIGHT - (GLfloat)ypos;
	//	iMouseY = yUpsideDown; //positive since it's clicked
	//	std::cout << "Cursor Position at (" << xpos << " : " << ypos << ")" << std::endl;
	//}
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::string vertex_shader_filename = "using_mouse.vs";
	std::string fragment_shader_filename = "using_mouse.fs";
	std::filesystem::path current_folder_path = std::filesystem::current_path();
	std::filesystem::path folder_name = current_folder_path.filename();
	//std::filesystem::path main_folder_name = current_folder_path.parent_path().filename();
	std::filesystem::path folder_base_path = current_folder_path.parent_path().parent_path().parent_path();
	std::filesystem::path resources_folder_full_path = folder_base_path / "resources";
	std::filesystem::path shaders_folder_full_path = resources_folder_full_path / "shaders";
	std::filesystem::path textures_folder_full_path = resources_folder_full_path / "textures";
	std::filesystem::path vertex_shader_file_full_path = shaders_folder_full_path / vertex_shader_filename;
	std::string vertex_shader_file_full_path_str = vertex_shader_file_full_path.string();
	std::filesystem::path fragment_shader_file_full_path = shaders_folder_full_path / fragment_shader_filename;
	std::string fragment_shader_file_full_path_str = fragment_shader_file_full_path.string();
	Shader ourShader(vertex_shader_file_full_path_str.c_str(), fragment_shader_file_full_path_str.c_str());
	

	float minVal = -0.9f;
	float maxVal = 0.9f;

	float vertices[] = {
		maxVal, maxVal, 0.0f,  // top right
		maxVal,	minVal, 0.0f,  // bottom right
		minVal, minVal, 0.0f,  // bottom left
		minVal, maxVal, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//char imagePath[] = "D:/Developments/OpenGL/resources/textures/rainforest.png";
	//char imagePath[] = "D:/Developments/OpenGL/resources/textures/dog.jpg";
	//initializeTextureAndLoadImage(imagePath);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//int frameCounter = 0;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// render the triangle
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//double  timeValueDouble = glfwGetTime();
		//float timeValue = static_cast<float>(timeValueDouble);
		//ourShader.setFloat("iTime", timeValue);

		unsigned int programID = ourShader.getID();
		int iResolutionLocation = glGetUniformLocation(programID, "iResolution");
		glUniform2f(iResolutionLocation, (GLfloat)SCR_WIDTH, (GLfloat)SCR_HEIGHT);

		int iMouseLocation = glGetUniformLocation(programID, "iMouse");
		glUniform4f(iMouseLocation, iMouseX, iMouseY, iMouseZ, iMouseW);

		//frameCounter++;
		//ourShader.setInt("iFrame", frameCounter);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}