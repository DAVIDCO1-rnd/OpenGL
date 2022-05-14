//------- Ignore this ----------
//#include<filesystem>
//namespace fs = std::filesystem;
//------------------------------



#include <string>

#include"Model.h"
#include "CircleModel.h"
#include "Model_GLTF.h"
#include "ModelParameters.h"

#include <IMGUI/imgui.h>
#include <IMGUI/imgui.cpp>
#include <IMGUI/imgui_draw.cpp>
#include <IMGUI/imgui_demo.cpp>
#include <IMGUI/imgui_tables.cpp>
#include <IMGUI/imgui_widgets.cpp>
#include <IMGUI/backends/imgui_impl_glfw.h>
#include <IMGUI/backends/imgui_impl_opengl3.h>

#include "Polygons.h"
#include <chrono>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
/*reads the pixels in the framebuffer and write them as BMP image to filename*/
void saveScreenShot(std::string filename, int WindowWidth, int windowHeight);
std::vector<std::vector<cv::Point>> calculatePolygons(size_t width, size_t height);
void drawBoundaryPoints(std::vector<std::vector<cv::Point>> contours);


const unsigned int width = 1280;
const unsigned int height = 720;
Camera* activeCamera;

namespace ImGuiGeneral
{
	void imGuiStartNewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void imGuiWrapperInitializeImGui(GLFWwindow* window, const char* glsl_version) {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void imGuiWrapperRender() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void imGuiWrapperCleanUp() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

namespace ImGuiModels
{
	void imGuiWrapperDisplayImGui(size_t& modelIndex, std::vector<Model*> models, float* clearColor, bool& renderMesh, CircleModel* circleModel) {
		// Start the Dear ImGui frame


		ImGui::Begin("Model parameters");
		{
			ImGui::ColorEdit3("clear color", clearColor); // Edit 3 floats representing a color

			std::vector<std::string> items;
			for (size_t i = 0; i < models.size(); i++)
			{
				items.push_back(models[i]->getModelName());
			}
			items.push_back(circleModel->getModelName());
			//string items[] = { meshes[0].getModelName(), meshes[1].getModelName()};

			if (ImGui::BeginListBox("models"))
			{
				for (size_t n = 0; n < items.size(); n++)
				{
					const bool is_selected = (modelIndex == n);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
						modelIndex = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					// if (is_selected)
					//     ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			{
				Model* activeModel;
				if (modelIndex < models.size())
				{
					activeModel = models[modelIndex];
				}
				else
				{
					activeModel = circleModel;
				}
				

				ModelParameters modelParams = activeModel->getParams();

				modelParams.angleX = activeModel->getParams().angleX;
				modelParams.angleY = activeModel->getParams().angleY;
				modelParams.angleZ = activeModel->getParams().angleZ;

				modelParams.translateX = activeModel->getParams().translateX;
				modelParams.translateY = activeModel->getParams().translateY;
				modelParams.translateZ = activeModel->getParams().translateZ;

				modelParams.scaleX = activeModel->getParams().scaleX;
				modelParams.scaleY = activeModel->getParams().scaleY;
				modelParams.scaleZ = activeModel->getParams().scaleZ;

				modelParams.scaleUniform = activeModel->getParams().scaleUniform;

				ImGui::InputFloat("rotate X", &modelParams.angleX, 1.0f, 1.0f);
				ImGui::InputFloat("rotate Y", &modelParams.angleY, 1.0f, 1.0f);
				ImGui::InputFloat("rotate Z", &modelParams.angleZ, 1.0f, 1.0f);

				ImGui::InputFloat("translate X", &modelParams.translateX, 0.1f, 1.0f);
				ImGui::InputFloat("translate Y", &modelParams.translateY, 0.1f, 1.0f);
				ImGui::InputFloat("translate Z", &modelParams.translateZ, 0.1f, 1.0f);

				ImGui::InputFloat("scale X", &modelParams.scaleX, 0.01f, 1.0f);
				ImGui::InputFloat("scale Y", &modelParams.scaleY, 0.01f, 1.0f);
				ImGui::InputFloat("scale Z", &modelParams.scaleZ, 0.01f, 1.0f);
				ImGui::InputFloat("uniform scale", &modelParams.scaleUniform, 0.01f, 1.0f);
				ImGui::Checkbox("render mesh", &renderMesh);

				activeModel->setParams(modelParams);
			}
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}


namespace ImGuiCameras
{
	void imGuiWrapperDisplayImGui(size_t& cameraIndex, std::vector<Camera*> cameras, float* clearColor) {
		// Start the Dear ImGui frame
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();

		//ImGui::NewFrame();

		ImGui::Begin("Camera parameters");
		{
			ImGui::ColorEdit3("clear color", clearColor); // Edit 3 floats representing a color

			std::vector<std::string> items;
			for (size_t i = 0; i < cameras.size(); i++)
			{
				items.push_back(cameras[i]->getCameraName());
			}
			//string items[] = { meshes[0].getModelName(), meshes[1].getModelName()};

			if (ImGui::BeginListBox("models"))
			{
				for (size_t n = 0; n < items.size(); n++)
				{
					const bool is_selected = (cameraIndex == n);
					if (ImGui::Selectable(items[n].c_str(), is_selected))
						cameraIndex = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					// if (is_selected)
					//     ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			{
				CameraParameters cameraParams = cameras[cameraIndex]->getParams();

				cameraParams.angleX = cameras[cameraIndex]->getParams().angleX;
				cameraParams.angleY = cameras[cameraIndex]->getParams().angleY;
				cameraParams.angleZ = cameras[cameraIndex]->getParams().angleZ;

				cameraParams.translateX = cameras[cameraIndex]->getParams().translateX;
				cameraParams.translateY = cameras[cameraIndex]->getParams().translateY;
				cameraParams.translateZ = cameras[cameraIndex]->getParams().translateZ;

				ImGui::InputFloat("rotate X", &cameraParams.angleX, 1.0f, 1.0f);
				ImGui::InputFloat("rotate Y", &cameraParams.angleY, 1.0f, 1.0f);
				ImGui::InputFloat("rotate Z", &cameraParams.angleZ, 1.0f, 1.0f);

				ImGui::InputFloat("translate X", &cameraParams.translateX, 0.1f, 1.0f);
				ImGui::InputFloat("translate Y", &cameraParams.translateY, 0.1f, 1.0f);
				ImGui::InputFloat("translate Z", &cameraParams.translateZ, 0.1f, 1.0f);

				cameras[cameraIndex]->setParams(cameraParams);
			}
		}
		if (ImGui::Button("Save framebuffer to file")) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
		{
			saveScreenShot("C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/saved_images/screenShot.bmp", width, height);
		}
		if (ImGui::Button("Calculate LOS polygons")) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
		{
			saveScreenShot("C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/saved_images/screenShot.bmp", width, height);
			std::vector<std::vector<cv::Point>> boundaryPoints = calculatePolygons(width, height);
			drawBoundaryPoints(boundaryPoints);
			std::vector<std::vector<glm::vec3>> worldCoords = activeCamera->convert2dPixelsTo3dWorldCoordinates(boundaryPoints);
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void drawBoundaryPoints(std::vector<std::vector<cv::Point>> contours) {
	//size_t numOfPolygons = contours.size();
	//for (size_t i = 0; i < numOfPolygons; i++) {
	//	std::vector<cv::Point> currentPolygon;
	//	size_t numOfPoints = currentPolygon.size();
	//	for (size_t j = 0; j < numOfPoints; j++) {
	//		cv::Point currentPoint = currentPolygon[j];
	//		int x = currentPoint.x;
	//		int y = currentPoint.y;
	//	}
	//}
}

unsigned char* convertRgbToBinaryImageRGB(unsigned char* rgbImage, int width, int height, unsigned char redCircleVal, unsigned char greenCircleVal, unsigned char blueCircleVal) {
	int nSize = 3 * width * height;
	unsigned char* binaryImage = new unsigned char[nSize];
	if (!binaryImage)
		return NULL;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < 3 * width; j+= 3) {
			size_t redIndex =	i * 3 * width + j + 0;
			size_t greenIndex =	i * 3 * width + j + 1;
			size_t blueIndex =	i * 3 * width + j + 2;

			unsigned char redVal =	rgbImage[redIndex];
			unsigned char greenVal =	rgbImage[greenIndex];
			unsigned char blueVal =	rgbImage[blueIndex];

			//size_t binaryIndex = i * height + j;
			if (redVal == redCircleVal && greenVal == greenCircleVal && blueVal == blueCircleVal) {
				binaryImage[redIndex] = 255;
				binaryImage[greenIndex] = 255;
				binaryImage[blueIndex] = 255;
			}
			else
			{
				binaryImage[redIndex] = 0;
				binaryImage[greenIndex] = 0;
				binaryImage[blueIndex] = 0;
			}
		}
	}
	return binaryImage;
}



std::vector<std::vector<cv::Point>> calculatePolygons(size_t width, size_t height) {
	auto startTime = std::chrono::steady_clock::now();
	std::vector<std::vector<cv::Point>> boundaryPoints = Polygons::calcPolygons(width, height);
	auto endTime = std::chrono::steady_clock::now();
	auto durationCaculatePolygons = endTime - startTime;
	auto durationCaculatePolygonsMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(durationCaculatePolygons).count();
	std::cout << "duration calculating polygons = " << durationCaculatePolygonsMilliseconds << " milliseconds" << std::endl;
	return boundaryPoints;

	//size_t numOfPolygons = boundaryPoints.size();
	//int counter = 0;
	//for (size_t polygonIndex = 0; polygonIndex < numOfPolygons; polygonIndex++)
	//{
	//	std::vector<std::vector<cv::Point>> currentPolygonPoints = boundaryPoints[polygonIndex];
	//	if (currentPolygonPoints.size() == 0) {
	//		//I think it can only happen when the foreground pixels are only one line or one column (and therefore no polygon to surround them)
	//		continue;
	//	}
	//	counter++;
	//	std::ofstream currentFile;
	//	std::string filePath = "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/matlab/Boundary_tracing_using_the_Moore_neighbourhood/polygons_folder/polygon" + std::to_string(counter) + ".csv";
	//	currentFile.open(filePath);
	//	for (size_t i = 0; i < currentPolygonPoints.size(); i++)
	//	{
	//		int xVal = currentPolygonPoints[i].X;
	//		int yVal = currentPolygonPoints[i].Y;
	//		currentFile << xVal << ", " << yVal << std::endl;
	//	}
	//	currentFile.close();
	//}

	MessageBox(0, "Finished calculating LOS polygons  ", "Los algorithm", MB_OK);
}


/*reads the pixels in the framebuffer and write them as BMP image to filename*/
void saveScreenShot(std::string filename, int WindowWidth, int windowHeight)
{
	int nSize = WindowWidth * windowHeight * 3;
	unsigned char* rgbImage = new unsigned char[nSize];
	if (!rgbImage)
		return;

	glReadPixels(0, 0, WindowWidth, windowHeight, GL_BGR, GL_UNSIGNED_BYTE, rgbImage);

	unsigned char redCircleVal = 255;
	unsigned char greenCircleVal = 0;
	unsigned char blueCircleVal = 0;
	unsigned char* binaryImageRGB = convertRgbToBinaryImageRGB(rgbImage, WindowWidth, windowHeight, redCircleVal, greenCircleVal, blueCircleVal);	
	
	delete[] rgbImage;

	FILE* Out = fopen(filename.c_str(), "wb");
	if (!Out)
	{
		std::cout << "Couldn't open " << filename << std::endl;
		return;
	}
		
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;

	bitmapFileHeader.bfType = 0x4D42;
	bitmapFileHeader.bfSize = nSize;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = WindowWidth;
	bitmapInfoHeader.biHeight = windowHeight;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB;
	bitmapInfoHeader.biSizeImage = 0;
	bitmapInfoHeader.biXPelsPerMeter = 0; // ?
	bitmapInfoHeader.biYPelsPerMeter = 0; // ?
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;

	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, Out);
	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, Out);
	fwrite(binaryImageRGB, nSize, 1, Out);
	fclose(Out);

	delete[] binaryImageRGB;
	//MessageBox(0, "Framebuffer was saved in saved_images folder  ", "Los algorithm", MB_OK);
}





int main()
{
	float clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
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
	GLFWwindow* window = glfwCreateWindow(width, height, "LOS algorithm", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	GLenum err011 = glGetError();
	if (err011 != GL_NO_ERROR)
	{
		std::cout << "err011" << std::endl;
		printOpenGLError(err011);
	}

	// Generates Shader object using shaders default.vert and default.frag
	Shader defaultShader("C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/src/shaders/default.vs", "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/src/shaders/default.fs");


	GLenum err012 = glGetError();
	if (err012 != GL_NO_ERROR)
	{
		std::cout << "err012" << std::endl;
		printOpenGLError(err012);
	}

	Shader shaderBlue("C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/src/shaders/shaderBlue.vs", "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/src/shaders/shaderBlue.fs");
	Shader shaderRed("C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/src/shaders/shaderRed.vs", "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/src/shaders/shaderRed.fs");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	defaultShader.Activate();
	glUniform4f(glGetUniformLocation(defaultShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(defaultShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	shaderRed.Activate();
	glUniform4f(glGetUniformLocation(shaderRed.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderRed.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);





	

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object

	glm::vec3 cameraPosition1 = glm::vec3(0.0f, 0.0f, 2.5f);
	float fov1 = 45.0f;
	float nearZ1 = 0.01f;
	float farZ1 = 1000.0f;
	std::string cameraName1 = "camera1";
	Camera camera1(width, height, cameraPosition1, fov1, nearZ1, farZ1, cameraName1);

	glm::vec3 cameraPosition2 = glm::vec3(0.0f, 0.0f, 2.0f);
	float fov2 = 45.0f;
	float nearZ2 = 0.01f;
	float farZ2 = 1000.0f;
	std::string cameraName2 = "camera2";
	Camera camera2(width, height, cameraPosition2, fov2, nearZ2, farZ2, cameraName2);

	std::vector<Camera*> cameras;
	cameras.push_back(&camera1);
	cameras.push_back(&camera2);

	size_t cameraIndex = 0;
	
	


	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	//std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string parentDir = "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1";

	std::string modelName1 = "bunny";
	std::string modelPath1 = "/Resources/models/" + modelName1 + "/scene.gltf";	

	//std::string modelName1 = "box_blender";
	//std::string modelPath1 = "/Resources/models/box_blender/box_blender.gltf";

	std::string modelName2 = "map";
	std::string modelPath2 = "/Resources/models/" + modelName2 + "/scene.gltf";

	// std::string modelName3 = "Avocado";
	// std::string modelPath3 = "/Resources/models/Avocado/glTF/Avocado.gltf";


	std::string modelName3 = "grindstone";
	std::string modelPath3 = "/Resources/models/" + modelName3 + "/scene.gltf";

	
	std::string modelName = "circle";
	ModelParameters params;
	int numOfAngles = 72;
	int numOfRadiuses = 2;
	float radius = 1.0f;
	float minRadius = 0.1f;        
	float circleCenterX = 0.0f;
	float circleCenterY = 0.0f;
	float zVal = -0.5f;  
	CircleModel* circleModel = new CircleModel(modelName, params, numOfAngles, radius, minRadius, numOfRadiuses, circleCenterX, circleCenterY, zVal);

	


	//std::string modelPath = "/Resources/models/Duck/glTF/Duck.gltf";
	//std::string modelPath = "/Resources/models/Avocado/glTF-Quantized/Avocado.gltf";
	//std::string modelPath = "/Resources/models/ToyCar/glTF/ToyCar.gltf";

	// Load in a model
	
	ModelParameters params1;
	params1.angleX = -92.0f;
	params1.angleY = 153.0f;
	params1.angleZ = 67.0f;
	params1.scaleUniform = 2.83f;
	Model_GLTF model1((parentDir + modelPath1).c_str(), modelName1, params1);


	
	ModelParameters params2;	
	//params2.angleY = -62.0f;

	params2.angleX = -17.0f;
	params2.angleY = -100.0f;
	params2.angleZ = 0.0f;

	params2.translateX = 1.0f;
	params2.translateY = -0.1f;
	params2.scaleUniform = 0.04f;
	Model_GLTF model2((parentDir + modelPath2).c_str(), modelName2, params2);


	
	ModelParameters params3;
	//params3.angleX = 68.0f;
	//params3.angleY = 0.0f;
	//params3.angleZ = 17.0f;
	params3.angleX = 60.0f;
	params3.angleY = 16.0f;
	params3.angleZ = 17.0f;

	params3.translateX = 0.2f;
	params3.translateY = 0.0f;
	params3.translateZ = -0.7f;
	params3.scaleUniform = 0.5f;
	Model_GLTF model3((parentDir + modelPath3).c_str(), modelName3, params3);



	

	std::vector<Model*> models;
	models.push_back(&model1);
	models.push_back(&model2);
	models.push_back(&model3);

	size_t modelIndex = 0;
	
	bool renderMesh = true;

	// Original code from the tutorial
	// Model model("models/bunny/scene.gltf");




	bool useImGui = true;

	if (useImGui) {
		ImGuiGeneral::imGuiWrapperInitializeImGui(window, glsl_version);
	}

	circleModel->generateBuffers();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		circleModel->saveBuffersForRedneringPolygones();
		if (useImGui) {
			ImGuiGeneral::imGuiStartNewFrame();
			ImGuiModels::imGuiWrapperDisplayImGui(modelIndex, models, clearColor, renderMesh, circleModel);
			ImGuiCameras::imGuiWrapperDisplayImGui(cameraIndex, cameras, clearColor);
		}
		activeCamera = cameras[cameraIndex];

		// Specify the color of the background
		glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		//camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader


        for (size_t i = 0 ; i < models.size() ; i++) {
            models[i]->updateModelMatrixByUserParameters();
        }
		circleModel->updateModelMatrixByUserParameters();

		activeCamera->updateViewMatrixByUserParameters();
		activeCamera->updateProjectionMatrixByUserParameters();
		
		
		

		//defaultShader.Activate();
		//for (size_t i = 0 ; i < models.size() ; i++) {
		//	models[i]->sendTransformationToVertexShader(defaultShader, activeCamera);
		//	models[i]->Draw(defaultShader, *activeCamera);
		//}

		shaderRed.Activate();
		for (size_t i = 0; i < models.size(); i++) {
			models[i]->sendTransformationToVertexShader(shaderRed, activeCamera);
			models[i]->Draw(shaderRed, *activeCamera);
		}

		glLineWidth(1.0);
		shaderBlue.Activate();
		circleModel->sendTransformationToVertexShader(shaderBlue, activeCamera);
		circleModel->Draw(shaderBlue, *activeCamera);

				

		if (useImGui) {
			ImGuiGeneral::imGuiWrapperRender();
		}	

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	defaultShader.Delete();
	shaderRed.Delete();
	shaderBlue.Delete();

	if (useImGui) {
		ImGuiGeneral::imGuiWrapperCleanUp();
	}

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}