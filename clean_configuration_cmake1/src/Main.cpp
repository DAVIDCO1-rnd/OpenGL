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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
/*reads the pixels in the framebuffer and write them as BMP image to filename*/
void saveScreenShot(string filename, int WindowWidth, int windowHeight);


const unsigned int width = 1280;
const unsigned int height = 720;

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
		if (ImGui::Button("Save framebuffer to file"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
		{
			saveScreenShot("C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/saved_images/screenShot.bmp", width, height);
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
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
//
//unsigned char* convertRgbToBinaryImage(unsigned char* rgbImage, int width, int height, unsigned char redCircleVal, unsigned char greenCircleVal, unsigned char blueCircleVal) {
//	int binaryImageSize = width * height;
//	unsigned char* binaryImage = new unsigned char[binaryImageSize];
//	if (!binaryImage)
//		return NULL;
//
//	size_t binaryIndex = 0;
//	for (size_t i = 0; i < height; i++)
//	{
//		for (size_t j = 0; j < 3 * width; j+=3) {
//			size_t redIndex =	i * 3 * width + j + 0;
//			size_t greenIndex =	i * 3 * width + j + 1;
//			size_t blueIndex =	i * 3 * width + j + 2;
//
//			unsigned char redVal =	rgbImage[redIndex];
//			unsigned char greenVal =	rgbImage[greenIndex];
//			unsigned char blueVal =	rgbImage[blueIndex];
//
//			if (redVal == redCircleVal && greenVal == greenCircleVal && blueVal == blueCircleVal) {
//				binaryImage[binaryIndex] = 255;
//			}
//			else {
//				binaryImage[binaryIndex] = 0;
//			}
//			binaryIndex++;
//		}
//	}
//	return binaryImage;
//}
//
//
//struct Point2D {
//	int X;
//	int Y;
//};
//
///*
//* Description - Get the continuous boundary points
//* Parameters
//* InputImage    - Input image
//* Width_i        - Width of the image
//* Height_i        - Height of Image
//* BoundaryPoints - Vector of boundary points (output)
//*/
//void GetContinousBoundaryPoints(unsigned char* InputImage, int Width_i, int Height_i, std::vector<Point2D>& BoundaryPoints)
//{
//	int nImageSize = Width_i * Height_i;
//	if (NULL != InputImage)
//	{
//		int Offset[8][2] = {
//								{ -1, -1 },       //  +----------+----------+----------+
//								{ 0, -1 },        //  |          |          |          |
//								{ 1, -1 },        //  |(x-1,y-1) | (x,y-1)  |(x+1,y-1) |
//								{ 1, 0 },         //  +----------+----------+----------+
//								{ 1, 1 },         //  |(x-1,y)   |  (x,y)   |(x+1,y)   |
//								{ 0, 1 },         //  |          |          |          |
//								{ -1, 1 },        //  +----------+----------+----------+
//								{ -1, 0 }         //  |          | (x,y+1)  |(x+1,y+1) |
//		};                    //  |(x-1,y+1) |          |          |
//							  //  +----------+----------+----------+
//		const int NEIGHBOR_COUNT = 8;
//		Point2D BoundaryPixelToSave; //david
//		Point2D BoundaryPixelCord;
//		Point2D BoundaryStartingPixelCord;
//		Point2D BacktrackedPixelCord;
//		int BackTrackedPixelOffset[1][2] = { {0,0} };
//		bool bIsBoundaryFound = false;
//		bool bIsStartingBoundaryPixelFound = false;
//		for (int Idx = 0; Idx < nImageSize; ++Idx) // getting the starting pixel of boundary
//		{
//			if (0 != InputImage[Idx])
//			{
//				BoundaryPixelCord.X = Idx % Width_i;
//				BoundaryPixelCord.Y = Idx / Width_i;
//				BoundaryStartingPixelCord = BoundaryPixelCord;
//				BacktrackedPixelCord.X = (Idx - 1) % Width_i;
//				BacktrackedPixelCord.Y = (Idx - 1) / Width_i;
//				BackTrackedPixelOffset[0][0] = BacktrackedPixelCord.X - BoundaryPixelCord.X;
//				BackTrackedPixelOffset[0][1] = BacktrackedPixelCord.Y - BoundaryPixelCord.Y;
//				BoundaryPixelToSave.Y = BoundaryPixelCord.X;
//				BoundaryPixelToSave.X = Height_i - BoundaryPixelCord.Y;
//				BoundaryPoints.push_back(BoundaryPixelToSave);
//				bIsStartingBoundaryPixelFound = true;
//				break;
//			}
//		}
//		Point2D CurrentBoundaryCheckingPixelCord;
//		Point2D PrevBoundaryCheckingPixxelCord;
//		if (!bIsStartingBoundaryPixelFound)
//		{
//			BoundaryPoints.pop_back();
//		}
//		while (true && bIsStartingBoundaryPixelFound)
//		{
//			int CurrentBackTrackedPixelOffsetInd = -1;
//			for (int Ind = 0; Ind < NEIGHBOR_COUNT; ++Ind)
//			{
//				if (BackTrackedPixelOffset[0][0] == Offset[Ind][0] &&
//					BackTrackedPixelOffset[0][1] == Offset[Ind][1])
//				{
//					CurrentBackTrackedPixelOffsetInd = Ind;// Finding the bracktracked 
//														   // pixel's offset index
//					break;
//				}
//			}
//			int Loop = 0;
//			while (Loop < (NEIGHBOR_COUNT - 1) && CurrentBackTrackedPixelOffsetInd != -1)
//			{
//				int OffsetIndex = (CurrentBackTrackedPixelOffsetInd + 1) % NEIGHBOR_COUNT;
//				CurrentBoundaryCheckingPixelCord.X = BoundaryPixelCord.X + Offset[OffsetIndex][0];
//				CurrentBoundaryCheckingPixelCord.Y = BoundaryPixelCord.Y + Offset[OffsetIndex][1];
//				int ImageIndex = CurrentBoundaryCheckingPixelCord.Y * Width_i +
//					CurrentBoundaryCheckingPixelCord.X;
//				if (0 != InputImage[ImageIndex])// finding the next boundary pixel
//				{
//					BoundaryPixelCord = CurrentBoundaryCheckingPixelCord;
//					BacktrackedPixelCord = PrevBoundaryCheckingPixxelCord;
//					BackTrackedPixelOffset[0][0] = BacktrackedPixelCord.X - BoundaryPixelCord.X;
//					BackTrackedPixelOffset[0][1] = BacktrackedPixelCord.Y - BoundaryPixelCord.Y;
//					BoundaryPixelToSave.Y = BoundaryPixelCord.X;
//					BoundaryPixelToSave.X = Height_i - BoundaryPixelCord.Y;
//					BoundaryPoints.push_back(BoundaryPixelToSave);
//					break;
//				}
//				PrevBoundaryCheckingPixxelCord = CurrentBoundaryCheckingPixelCord;
//				CurrentBackTrackedPixelOffsetInd += 1;
//				Loop++;
//			}
//			if (BoundaryPixelCord.X == BoundaryStartingPixelCord.X &&
//				BoundaryPixelCord.Y == BoundaryStartingPixelCord.Y) // if the current pixel = 
//																	 // starting pixel
//			{
//				BoundaryPoints.pop_back();
//				bIsBoundaryFound = true;
//				break;
//			}
//		}
//		if (!bIsBoundaryFound) // If there is no connected boundary clear the list
//		{
//			BoundaryPoints.clear();
//		}
//	}
//}

//unsigned char* convertBinaryImageToLabelsImage(unsigned char* rgbImage, int width, int height) {
//	std::vector<
//}
//
//std::vector<Point2D> calcPolygons(int width, int height) {
//	std::vector<Point2D> BoundaryPoints;
//	int nSize = width * height * 3;
//	unsigned char* rgbImage = new unsigned char[nSize];
//	if (!rgbImage)
//		return BoundaryPoints;
//
//	glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, rgbImage);
//
//	unsigned char redCircleVal = 255;
//	unsigned char greenCircleVal = 0;
//	unsigned char blueCircleVal = 0;
//	
//	unsigned char* binaryImage = convertRgbToBinaryImage(rgbImage, width, height, redCircleVal, greenCircleVal, blueCircleVal);
//	//unsigned char* labelsImage = convertBinaryImageToLabelsImage(binaryImage, width, height);
//	
//	GetContinousBoundaryPoints(binaryImage, width, height, BoundaryPoints);
//
//	delete[] rgbImage;
//	delete[] binaryImage;
//
//	return BoundaryPoints;
//}


/*reads the pixels in the framebuffer and write them as BMP image to filename*/
void saveScreenShot(string filename, int WindowWidth, int windowHeight)
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
		cout << "Couldn't open " << filename << endl;
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
	std::vector<Polygons::Point2D> boundaryPoints = Polygons::calcPolygons(width, height);
	ofstream myfile;
	myfile.open("C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/matlab/Boundary_tracing_using_the_Moore_neighbourhood/david.csv");
	for (int i = 0; i < boundaryPoints.size(); i++)
	{
		int xVal = boundaryPoints[i].X;
		int yVal = boundaryPoints[i].Y;
		myfile << xVal << ", " << yVal << endl;
	}
	myfile.close();
	MessageBox(0, "Framebuffer was saved in saved_images folder  ", "Los algorithm", MB_OK);
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
	Camera* activeCamera;
	


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

	
	string modelName = "circle";
	ModelParameters params;
	int numOfAngles = 72;
	int numOfRadiuses = 2;
	float radius = 0.8f;
	float minRadius = 0.1f;        
	float circleCenterX = 0.0f;
	float circleCenterY = 0.0f;
	float zVal = -0.4f;  
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
	params2.angleY = -62.0f;
	params2.translateX = 1.0f;
	params2.translateY = -0.1f;
	params2.scaleUniform = 0.04f;
	Model_GLTF model2((parentDir + modelPath2).c_str(), modelName2, params2);


	
	ModelParameters params3;
	params3.angleX = 68.0f;
	params3.angleY = 0.0f;
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