//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include <IMGUI/imgui.h>
#include <IMGUI/imgui.cpp>
#include <IMGUI/imgui_draw.cpp>
#include <IMGUI/imgui_demo.cpp>
#include <IMGUI/imgui_tables.cpp>
#include <IMGUI/imgui_widgets.cpp>
#include <IMGUI/backends/imgui_impl_glfw.h>
#include <IMGUI/backends/imgui_impl_opengl3.h>

#include <string>

#include"Model.h"
#include "ModelParameters.h"



const unsigned int width = 800;
const unsigned int height = 800;

// void displayImGui(size_t item_current_idx, std::vector<Model*> models, ImVec4 clear_color, bool renderMesh) {
//         // Start the Dear ImGui frame
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();

//         ImGui::NewFrame();


//         std::string modelName = models[item_current_idx]->getModelName();
//         ImGui::Begin(modelName.c_str());
//         {
//             ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

//             std::vector<std::string> items;
//             for (size_t i=0 ; i<models.size() ; i++)
//             {
//                 items.push_back(models[i]->getModelName());
//             }            
//             //string items[] = { meshes[0].getModelName(), meshes[1].getModelName()};
            
//             if (ImGui::BeginListBox("models"))
//             {
//                 for (size_t n = 0; n < items.size(); n++)
//                 {
//                     const bool is_selected = (item_current_idx == n);
//                     if (ImGui::Selectable(items[n].c_str(), is_selected))
//                         item_current_idx = n;

//                     // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//                     // if (is_selected)
//                     //     ImGui::SetItemDefaultFocus();
//                 }
//                 ImGui::EndListBox();
//             }          

//             {
//                 ModelParameters meshParams = models[item_current_idx]->getParams();

//                 meshParams.angleX = models[item_current_idx]->getParams().angleX;
//                 meshParams.angleY = models[item_current_idx]->getParams().angleY;
//                 meshParams.angleZ = models[item_current_idx]->getParams().angleZ;

//                 meshParams.translateX = models[item_current_idx]->getParams().translateX;
//                 meshParams.translateY = models[item_current_idx]->getParams().translateY;
//                 meshParams.translateZ = models[item_current_idx]->getParams().translateZ;

//                 meshParams.scaleX = models[item_current_idx]->getParams().scaleX;
//                 meshParams.scaleY = models[item_current_idx]->getParams().scaleY;
//                 meshParams.scaleZ = models[item_current_idx]->getParams().scaleZ;

//                 meshParams.scaleUniform = models[item_current_idx]->getParams().scaleUniform;

//                 ImGui::InputFloat("rotate X", &meshParams.angleX, 1.0f, 1.0f);
//                 ImGui::InputFloat("rotate Y", &meshParams.angleY, 1.0f, 1.0f);
//                 ImGui::InputFloat("rotate Z", &meshParams.angleZ, 1.0f, 1.0f);

//                 ImGui::InputFloat("translate X", &meshParams.translateX, 0.1f, 1.0f);
//                 ImGui::InputFloat("translate Y", &meshParams.translateY, 0.1f, 1.0f);
//                 ImGui::InputFloat("translate Z", &meshParams.translateZ, 0.1f, 1.0f);

//                 ImGui::InputFloat("scale X", &meshParams.scaleX, 0.01f, 1.0f);
//                 ImGui::InputFloat("scale Y", &meshParams.scaleY, 0.01f, 1.0f);
//                 ImGui::InputFloat("scale Z", &meshParams.scaleZ, 0.01f, 1.0f);
//                 ImGui::InputFloat("uniform scale", &meshParams.scaleUniform, 0.01f, 1.0f);
//                 ImGui::Checkbox("render mesh", &renderMesh);

//                 models[item_current_idx]->setParams(meshParams);                
//             }
//         }
//         ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//         ImGui::End(); 
// }

// void initializeImGui(GLFWwindow* window, const char* glsl_version) {
//     // Setup Dear ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//     //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

//     // Setup Dear ImGui style
//     ImGui::StyleColorsDark();
//     //ImGui::StyleColorsClassic();

//     // Setup Platform/Renderer backends
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init(glsl_version);
// }


int main()
{
	// Initialize GLFW
	glfwInit();

	const char* glsl_version = "#version 130";
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);





	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("/home/dell/Developments/OpenGL/clean_configuration_cmake1/src/shaders/default.vs", "/home/dell/Developments/OpenGL/clean_configuration_cmake1/src/shaders/default.fs");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();

	std::string modelPath = "/Resources/models/bunny/scene.gltf";
	//std::string modelPath = "/Resources/models/cut_fish/scene.gltf";
	//std::string modelPath = "/Resources/models/map/scene.gltf";
	//std::string modelPath = "/Resources/models/Box/glTF/Box.gltf";
	//std::string modelPath = "/Resources/models/Duck/glTF/Duck.gltf";
	//std::string modelPath = "/Resources/models/Avocado/glTF-Quantized/Avocado.gltf";
	//std::string modelPath = "/Resources/models/ToyCar/glTF/ToyCar.gltf";

	// Load in a model
	Model model((parentDir + modelPath).c_str(), "bunny");

	std::vector<Model*> models;
	models.push_back(&model);
	size_t item_current_idx = 0;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool renderMesh = true;

	// Original code from the tutorial
	// Model model("models/bunny/scene.gltf");

	//initializeImGui(window, glsl_version);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		//displayImGui(item_current_idx, models, clear_color, renderMesh);


		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Draw a model
		model.Draw(shaderProgram, camera);

        // // Rendering
        // ImGui::Render();
        // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());		

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();

    // // Cleanup
    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}