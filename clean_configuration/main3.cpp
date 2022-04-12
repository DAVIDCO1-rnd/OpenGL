// ImGui - standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

// #include <algorithm>
// #include "imgui.h"
// #include "imgui_impl_glfw_gl3.h"
// #include <stdio.h>
// #include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
// #include <nfd.h>
// #include "targilon.h"
// #include "MeshPlane.h"
// #include "MeshPlaneIntersection.h"

#include <IMGUI/imgui.h>
#include <IMGUI/imgui.cpp>
#include <IMGUI/imgui_draw.cpp>
#include <IMGUI/imgui_demo.cpp>
#include <IMGUI/imgui_tables.cpp>
#include <IMGUI/imgui_widgets.cpp>
#include <IMGUI/backends/imgui_impl_glfw.h>
#include <IMGUI/backends/imgui_impl_opengl3.h>



vector<vector<float>> calcVerticesIntersectingPlane(Vec3d planeNormal, Mat1d vertices, Mat1i faces, double signedDistOfPlaneFromOrigin)
{
	vector<vector<float>> meshPlaneVerticesIntersection;
	Vec3d planeUnitNormal = normalizeVec(planeNormal);
	Mat rotation = calcRotation(planeUnitNormal);
	Mat transposedRotation = rotation.t();
	Mat1d rotatedVertices = calcRotatedVertices(vertices, rotation);
	int numOfTriangles = faces.rows;

	for (int i = 0; i < numOfTriangles; i++)
	{
		int index1 = faces.at<int>(i, 0);
		int index2 = faces.at<int>(i, 1);
		int index3 = faces.at<int>(i, 2);

		Vec3d point1 = Vec3d(rotatedVertices.at<double>(index1 - 0, 0), rotatedVertices.at<double>(index1 - 0, 1), rotatedVertices.at<double>(index1 - 0, 2));
		Vec3d point2 = Vec3d(rotatedVertices.at<double>(index2 - 0, 0), rotatedVertices.at<double>(index2 - 0, 1), rotatedVertices.at<double>(index2 - 0, 2));
		Vec3d point3 = Vec3d(rotatedVertices.at<double>(index3 - 0, 0), rotatedVertices.at<double>(index3 - 0, 1), rotatedVertices.at<double>(index3 - 0, 2));
		vector<Vec3d> triangle{ point1, point2, point3 };
		Mat1d intersectionForStandardAxis = calcIntersectionBetweenTriangleAndPlane(triangle, signedDistOfPlaneFromOrigin);
		size_t numOfIntersections = intersectionForStandardAxis.rows;
		Mat1d intersectionForStandardAxisTransposed;
		Mat1d transposedIntersection;
		Mat1d intersection;
		if (numOfIntersections > 0)
		{
			intersectionForStandardAxisTransposed = intersectionForStandardAxis.t();
			transposedIntersection = (transposedRotation * intersectionForStandardAxisTransposed);
			intersection = transposedIntersection.t();
		}
		for (size_t j = 0; j < numOfIntersections; j++)
		{
			for (size_t k = 1; k < numOfIntersections; k++)
			{
				Vec3d p1 = Vec3d(intersection.at<double>(j, 0), intersection.at<double>(j, 1), intersection.at<double>(j, 2));
				Vec3d p2 = Vec3d(intersection.at<double>(k, 0), intersection.at<double>(k, 1), intersection.at<double>(k, 2));

				vector<float> point1ToPoint2{ (float)p1[0], (float)p1[1], (float)p1[2], (float)p2[0], (float)p2[1], (float)p2[2] };
				meshPlaneVerticesIntersection.push_back(point1ToPoint2);
			}
		}
	}

	return meshPlaneVerticesIntersection;
}


void mainOpenCV(string fileName, Mat1i faces, Mat1d vertices, string filePath, MeshPlaneIntersection& meshPlaneIntersection, Vec3d planeNormal, double signedDistOfPlaneFromOrigin, int width, int height, int gridX, int gridY)
{
	vector<vector<float>> meshPlaneVerticesIntersection = calcVerticesIntersectingPlane(planeNormal, vertices, faces, signedDistOfPlaneFromOrigin);
	meshPlaneIntersection.meshPlaneVerticesIntersection = meshPlaneVerticesIntersection;
	static int counter = 0;


	//std::cout << "Calculating the contour image. Please wait..." << std::endl;
	clock_t begin = clock();
	Mat1i counterImage = createContour(faces, vertices, planeNormal, signedDistOfPlaneFromOrigin, width, height, gridX, gridY);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	//std::cout << "Calculating the contour image took " << elapsed_secs << " seconds" << std::endl;


	counter++;
	string imageName = fileName + "_" + std::to_string(width) + "_" + std::to_string(height) + "___" + std::to_string(counter);
	string folderPath = "saved_images";
	string imageFullPath = "./" + folderPath + "/" + imageName + ".jpg";
	imwrite(imageFullPath, counterImage);

	string figureTitle = "Contour of mesh";
	imshow(figureTitle, counterImage);

	//waitKey(0);
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}



int main(int, char**)
{
	//string fileName = "room";
	//string filePath = "C:/Users/davidco1/Documents/David/MeshRendering/NewData/" + fileName + ".obj";
	string fileName = "room";
	string filePath = "../../Data/" + fileName + ".obj";
	Vec3d planeNormal(0, 0, 1);
	float signedDistOfPlaneFromOrigin = -4.0f;
	int width = 600;
	int height = 800;
	int gridX = 600;
	int gridY = 800;
	bool renderMesh = true;
	bool renderIntersection = true;
	bool calcIntersection = true;
	Mat1i faces;
	Mat1d vertices;
	bool fileReadStatus = loadOBJ(filePath, vertices, faces);
	if (!fileReadStatus)
	{
		std::cout << "Failed to read file!" << std::endl;
		return -1;
	}

	Mesh myMesh(filePath);
	MeshPlaneIntersection meshPlaneIntersection;
	/*mainOpenCV(fileName, filePath, meshPlaneIntersection, planeNormal, signedDistOfPlaneFromOrigin, width, height, gridX, gridY);*/
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rafael - OpenGL targilon", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	Shader shaderWithoutFilters("resources/shaders/shaderWithoutFilters.vs", "resources/shaders/shaderWithoutFilters.fs");
	Shader shaderRed("resources/shaders/shaderRed.vs", "resources/shaders/shaderRed.fs");
	Shader shaderAxis("resources/shaders/shaderAxis.vs", "resources/shaders/shaderAxis.fs");
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	saveBuffersForRedneringWholeMesh(&myMesh);
	/*saveBuffersForRedneringIntersectionBetweenMeshAndPlane(&meshPlaneIntersection);*/

	float lastDist = signedDistOfPlaneFromOrigin;
	
	GLfloat planeAngleX = 0.0f;
	GLfloat planeAngleY = 0.0f;
	GLfloat planeAngleZ = 0.0f;

	GLfloat lastPlaneRotationX = 0.0f;
	GLfloat lastPlaneRotationY = 0.0f;
	GLfloat lastPlaneRotationZ = 0.0f;

	glm::mat4 planeRotation; //identity matrix
	Vec3d rotatedPlaneNormal = planeNormal;
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
		//GLenum err = glGetError();
		//if (err != GL_NO_ERROR)
		//{
		//	printOpenGLError(err);
		//}

		if (!checkIfNumbersAreEqual(lastDist, signedDistOfPlaneFromOrigin))
		{
			calcIntersection = true;
			lastDist = signedDistOfPlaneFromOrigin;
		}

		if (!checkIfNumbersAreEqual(planeAngleX, lastPlaneRotationX) ||
			!checkIfNumbersAreEqual(planeAngleY, lastPlaneRotationY) ||
			!checkIfNumbersAreEqual(planeAngleZ, lastPlaneRotationZ))
		{
			calcPlaneRotationMatrix(planeRotation, planeAngleX, planeAngleY, planeAngleZ);
			lastPlaneRotationX = planeAngleX;
			lastPlaneRotationY = planeAngleY;
			lastPlaneRotationZ = planeAngleZ;
			glm::vec4 vec = glm::vec4(planeNormal[0], planeNormal[1], planeNormal[2], 1.0);
			glm::vec4 rotatedVec = planeRotation * vec;
			rotatedPlaneNormal[0] = rotatedVec[0];
			rotatedPlaneNormal[1] = rotatedVec[1];
			rotatedPlaneNormal[2] = rotatedVec[2];
			calcIntersection = true;
		}

		

		if (calcIntersection)
		{
			mainOpenCV(fileName, faces, vertices, filePath, meshPlaneIntersection, rotatedPlaneNormal, signedDistOfPlaneFromOrigin, width, height, gridX, gridY);
			saveBuffersForRedneringIntersectionBetweenMeshAndPlane(&meshPlaneIntersection);
			calcIntersection = false;
		}

        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // 1. Show a simple window.
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		//ImGui::Begin("", &open, ImVec2(576, 680), 0.9f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
		ImGui::Begin("Hello, Rafael!");
		{
            static float f = 0.0f;
            static int counter = 0;
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
			{
				ImGui::InputFloat("rotate X", &params.angleX, 1.0f, 1.0f);
				ImGui::InputFloat("rotate Y", &params.angleY, 1.0f, 1.0f);
				ImGui::InputFloat("rotate Z", &params.angleZ, 1.0f, 1.0f);

				ImGui::InputFloat("translate X", &params.translateX, 0.1f, 1.0f);
				ImGui::InputFloat("translate Y", &params.translateY, 0.1f, 1.0f);
				ImGui::InputFloat("translate Z", &params.translateZ, 0.1f, 1.0f);

				ImGui::InputFloat("scale X", &params.scaleX, 0.01f, 1.0f);
				ImGui::InputFloat("scale Y", &params.scaleY, 0.01f, 1.0f);
				ImGui::InputFloat("scale Z", &params.scaleZ, 0.01f, 1.0f);
				ImGui::InputFloat("uniform scale", &params.scaleUniform, 0.01f, 1.0f);
			}
			{
				ImGui::Checkbox("render mesh", &renderMesh);
				ImGui::Checkbox("render intersection", &renderIntersection);
				
				//ImGui::SliderFloat("signed distance of plane from origin", &signedDistOfPlaneFromOrigin, -80.0f, 80.0f);
				//ImGui::SliderFloat("rotate X plane", &planeAngleX, -90.0f, 90.0f);
				//ImGui::SliderFloat("rotate Y plane", &planeAngleY, -90.0f, 90.0f);
				//ImGui::SliderFloat("rotate Z plane", &planeAngleZ, -90.0f, 90.0f);

				ImGui::InputFloat("signed distance of plane from origin", &signedDistOfPlaneFromOrigin, 0.1f, 1.0f);
				ImGui::InputFloat("rotate X plane", &planeAngleX, 1.0f, 1.0f);
				ImGui::InputFloat("rotate Y plane", &planeAngleY, 1.0f, 1.0f);
				ImGui::InputFloat("rotate Z plane", &planeAngleZ, 1.0f, 1.0f);
				float planeNormalArray[3] = { rotatedPlaneNormal[0], rotatedPlaneNormal[1], rotatedPlaneNormal[2] };
				ImGui::InputFloat3("Plane Normal", planeNormalArray);
			}
        }
		ImGui::End();

        // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }

		// 4. Demonstrate creating a fullscreen menu bar and populating it.
		{
			ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Load Mesh...", "CTRL+O"))
					{
						nfdchar_t *outPath = NULL;
						nfdresult_t result = NFD_OpenDialog("obj", NULL, &outPath);
						if (result == NFD_OKAY) {
							bool fileReadStatus = loadOBJ(outPath, vertices, faces);
							deleteMeshBuffers(&myMesh);
							myMesh.loadMesh(outPath);
							saveBuffersForRedneringWholeMesh(&myMesh);
							mainOpenCV(outPath, faces, vertices, filePath, meshPlaneIntersection, rotatedPlaneNormal, signedDistOfPlaneFromOrigin, width, height, gridX, gridY);
							saveBuffersForRedneringIntersectionBetweenMeshAndPlane(&meshPlaneIntersection);
							free(outPath);
							if (!fileReadStatus)
							{
								std::cout << "Failed to read file!" << std::endl;
								return -1;
							}							
						}
						else if (result == NFD_CANCEL) {
						}
						else {
						}

					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 model, view, projection;
		updateModelViewByUserParameters(model);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		if (renderMesh)
		{
			glLineWidth(1.0);
			shaderWithoutFilters.use();
			sendTransformationToVertexShader(shaderWithoutFilters, model, view, projection);
			renderObject(&myMesh);
		}


		if (renderIntersection)
		{
			glLineWidth(7.0);
			shaderRed.use();
			sendTransformationToVertexShader(shaderRed, model, view, projection);
			renderIntesection(&meshPlaneIntersection);
		}



        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);        
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}



