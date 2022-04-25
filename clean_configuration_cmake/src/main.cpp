// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui.cpp>
#include <IMGUI/imgui_draw.cpp>
#include <IMGUI/imgui_demo.cpp>
#include <IMGUI/imgui_tables.cpp>
#include <IMGUI/imgui_widgets.cpp>
#include <IMGUI/backends/imgui_impl_glfw.h>
#include <IMGUI/backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp" //for glm::rotate, glm::translate, glm::scale



#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Shaders/shader_t.h"

#include "RayMesh.h"
#include "CircleMesh.h"
#include "ModelParameters.h"
#include "Camera.h"



//#include <filesystem>
//namespace fs = std::filesystem;


#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
//#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <string>
#include "Mesh.h"

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);




const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;



glm::vec3 camera1Pos   = glm::vec3(0.0f, 0.0f,  -20.0f);
glm::vec3 camera1Front = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 camera1Up    = glm::vec3(0.0f, 1.0f,  0.0f);
float camera1Angle = 45.0f;
float camera1Near = 0.1f;
float camera1Far = 1000.0f;
Camera camera1(camera1Pos, camera1Front, camera1Up, camera1Angle, camera1Near, camera1Far, SCR_WIDTH, SCR_HEIGHT);


glm::vec3 camera2Pos   = glm::vec3(0.0f, 30.0f,  -20.0f);
glm::vec3 camera2Front = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 camera2Up    = glm::vec3(0.0f, 1.0f,  0.0f);
float camera2Angle = 45.0f;
float camera2Near = 0.1f;
float camera2Far = 1000.0f;
Camera camera2(camera2Pos, camera2Front, camera2Up, camera2Angle, camera2Near, camera2Far, SCR_WIDTH, SCR_HEIGHT);


std::vector<Camera*> cameras;
size_t cameraIndex = 0;





// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

/*saving the matrices relevant for transformation to the vertex shader*/
void sendTransformationToVertexShader(Shader shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
}

void calcLOS(CircleMesh& circleMesh, size_t circleIndex, std::vector<Mesh*> meshes) {
    std::vector<float> circleVertices = circleMesh.getVerticesInModelCoordinates();
    int numOfVerticesInCircle = circleVertices.size() / 3;
    std::vector<bool> circlePointsWithLos(numOfVerticesInCircle, false);
    for (int i=0 ; i<numOfVerticesInCircle ; i++)
    {
        double circleX = (double)circleVertices[3*i + 0];
        double circleY = (double)circleVertices[3*i + 1];
        double circleZ = (double)circleVertices[3*i + 2];
        cv::Vec3d currentCirclePoint = cv::Vec3d(circleX, circleY, circleZ);
        cv::Vec3d point(camera1Pos[0], camera1Pos[1], camera1Pos[2]);
        cv::Vec3d vec = currentCirclePoint - point;
        Ray ray(point, vec);
        RayMesh rayMesh(ray);

        cv::Vec3d intersectionPoint;
        double min_t; 
        size_t meshIntersectionIndex;   
        bool isIntersecting = rayMesh.calcIntersectionBetweenRayAndMultiplesMeshes(meshes, intersectionPoint, min_t, meshIntersectionIndex);
        if (isIntersecting == true && meshIntersectionIndex == circleIndex)
        {
            circlePointsWithLos[i] = true;
        }  
    }

    vector<unsigned int> circleTrianglesIndicesWithLos;
    vector<unsigned int> circleTrianglesIndicesWithoutLos;
    vector<unsigned int> circleTrianglesIndices = circleMesh.getIndices();
    int numOfTrianglesInCircle = circleTrianglesIndices.size() / 3;
    for (int i=0 ; i<numOfTrianglesInCircle ; i++)
    {
        int circleVertexIndex1 = circleTrianglesIndices[3*i + 0];
        int circleVertexIndex2 = circleTrianglesIndices[3*i + 1];
        int circleVertexIndex3 = circleTrianglesIndices[3*i + 2];

        if (circlePointsWithLos[circleVertexIndex1] && circlePointsWithLos[circleVertexIndex2] && circlePointsWithLos[circleVertexIndex3])
        {
            circleTrianglesIndicesWithLos.push_back(circleVertexIndex1);
            circleTrianglesIndicesWithLos.push_back(circleVertexIndex2);
            circleTrianglesIndicesWithLos.push_back(circleVertexIndex3);
        }
        else
        {
            circleTrianglesIndicesWithoutLos.push_back(circleVertexIndex1);
            circleTrianglesIndicesWithoutLos.push_back(circleVertexIndex2);
            circleTrianglesIndicesWithoutLos.push_back(circleVertexIndex3);
        }
    }    

    circleMesh.indicesWithLos(circleTrianglesIndicesWithLos);
    circleMesh.indicesWithoutLos(circleTrianglesIndicesWithoutLos);
}

void displayImGui(bool show_demo_window, std::vector<Mesh*> meshes, ImVec4 clear_color, bool renderMesh, bool show_another_window) {
        static size_t item_current_idx = 0; // Here we store our selection data as an index.
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        string modelName = meshes[item_current_idx]->getModelName();
        ImGui::Begin(modelName.c_str());
        {
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            vector<string> items;
            for (size_t i=0 ; i<meshes.size() ; i++)
            {
                items.push_back(meshes[i]->getModelName());
            }            
            //string items[] = { meshes[0].getModelName(), meshes[1].getModelName()};
            
            if (ImGui::BeginListBox("models"))
            {
                for (size_t n = 0; n < items.size(); n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(items[n].c_str(), is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    // if (is_selected)
                    //     ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }          

            {
                ModelParameters meshParams = meshes[item_current_idx]->getParams();

                meshParams.angleX = meshes[item_current_idx]->getParams().angleX;
                meshParams.angleY = meshes[item_current_idx]->getParams().angleY;
                meshParams.angleZ = meshes[item_current_idx]->getParams().angleZ;

                meshParams.translateX = meshes[item_current_idx]->getParams().translateX;
                meshParams.translateY = meshes[item_current_idx]->getParams().translateY;
                meshParams.translateZ = meshes[item_current_idx]->getParams().translateZ;

                meshParams.scaleX = meshes[item_current_idx]->getParams().scaleX;
                meshParams.scaleY = meshes[item_current_idx]->getParams().scaleY;
                meshParams.scaleZ = meshes[item_current_idx]->getParams().scaleZ;

                meshParams.scaleUniform = meshes[item_current_idx]->getParams().scaleUniform;

                ImGui::InputFloat("rotate X", &meshParams.angleX, 1.0f, 1.0f);
                ImGui::InputFloat("rotate Y", &meshParams.angleY, 1.0f, 1.0f);
                ImGui::InputFloat("rotate Z", &meshParams.angleZ, 1.0f, 1.0f);

                ImGui::InputFloat("translate X", &meshParams.translateX, 0.1f, 1.0f);
                ImGui::InputFloat("translate Y", &meshParams.translateY, 0.1f, 1.0f);
                ImGui::InputFloat("translate Z", &meshParams.translateZ, 0.1f, 1.0f);

                ImGui::InputFloat("scale X", &meshParams.scaleX, 0.01f, 1.0f);
                ImGui::InputFloat("scale Y", &meshParams.scaleY, 0.01f, 1.0f);
                ImGui::InputFloat("scale Z", &meshParams.scaleZ, 0.01f, 1.0f);
                ImGui::InputFloat("uniform scale", &meshParams.scaleUniform, 0.01f, 1.0f);
                ImGui::Checkbox("render mesh", &renderMesh);

                meshes[item_current_idx]->setParams(meshParams);                
            }
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }    
}

void initializeImGui(GLFWwindow* window, const char* glsl_version) {
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


int main(int, char**)
{
    cameras.push_back(&camera1);
    //cameras.push_back(&camera2);
    size_t circleIndex = 0;
    string modelsFolder = "/home/dell/Developments/OpenGL/clean_configuration_cmake/Data/"; 

    string modelName1 = "circle";
    string filePath1 = modelsFolder + modelName1 + ".obj";
    ModelParameters meshParams1;
    CircleMesh circleMesh(modelName1, filePath1, meshParams1);

     

    string modelName2 = "bunny";
    string filePath2 = modelsFolder + modelName2 + ".obj";
    ModelParameters meshParams2;
    meshParams2.angleZ = 30;
    meshParams2.scaleUniform = 0.05f;
    Mesh mesh2(modelName2, filePath2, meshParams2); 

    string modelName3 = "room";
    string filePath3 = modelsFolder + modelName3 + ".obj";
    ModelParameters meshParams3;
    meshParams3.translateY = 30;
    meshParams3.translateZ = 100;
    Mesh mesh3(modelName3, filePath3, meshParams3);     
         

    std::vector<Mesh*> meshes;
    meshes.push_back(&circleMesh);  
    meshes.push_back(&mesh2);
    meshes.push_back(&mesh3);


    

	bool renderMesh = true;
	



    // Setup window
    //glfwSetErrorCallback(glfw_error_callback);
    int glfwInitStatus = glfwInit();
    if (glfwInitStatus == 0)
    {
        return 1;
    }
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Checking LOS Algorithm", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //initializeImGui(window, glsl_version);



    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Shader shaderWithoutFilters("/home/dell/Developments/OpenGL/clean_configuration_cmake/src/shaders/shaderWithoutFilters.vs", "/home/dell/Developments/OpenGL/clean_configuration_cmake/src/shaders/shaderWithoutFilters.fs");
	Shader shaderRed("/home/dell/Developments/OpenGL/clean_configuration_cmake/src/shaders/shaderRed.vs", "/home/dell/Developments/OpenGL/clean_configuration_cmake/src/shaders/shaderRed.fs");
	Shader shaderBlue("/home/dell/Developments/OpenGL/clean_configuration_cmake/src/shaders/shaderBlue.vs", "/home/dell/Developments/OpenGL/clean_configuration_cmake/src/shaders/shaderBlue.fs");

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    calcLOS(circleMesh, circleIndex, meshes);
    circleMesh.generateBuffers();

    for (size_t i=1 ; i<meshes.size() ; i++)
    {
        meshes[i]->saveBuffersForRedneringWholeMesh(); 
    }

    initializeImGui(window, glsl_version);
    // Main loop
    while (!glfwWindowShouldClose(window))
    {        
        circleMesh.saveBuffersForRedneringPolygonesWithLOS();
        circleMesh.saveBuffersForRedneringPolygonesWithoutLOS();        
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        

        displayImGui(show_demo_window, meshes, clear_color, renderMesh, show_another_window);




        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);      
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // // camera
        // glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  250.0f);
        // glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        // glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
		// glm::mat4 projection;
        // glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // projection = glm::mat4(1.0);		
		// projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        for (size_t i = 0 ; i < meshes.size() ; i++) {
            meshes[i]->updateModelMatrixByUserParameters();
        }        

		if (renderMesh)
		{
			glLineWidth(1.0); 

			shaderRed.use();            
            sendTransformationToVertexShader(shaderRed, circleMesh.getModelMatrix(), cameras[cameraIndex]->view(), cameras[cameraIndex]->projection());
            circleMesh.renderPolygones(CircleMesh::PolygonsWithLos);  

			shaderBlue.use();            
            sendTransformationToVertexShader(shaderBlue, circleMesh.getModelMatrix(), cameras[cameraIndex]->view(), cameras[cameraIndex]->projection());            
            circleMesh.renderPolygones(CircleMesh::PolygonsWithoutLos);

            shaderWithoutFilters.use();   
            for (size_t i = 1 ; i < meshes.size() ; i++) {
                sendTransformationToVertexShader(shaderWithoutFilters, meshes[i]->getModelMatrix(), cameras[cameraIndex]->view(), cameras[cameraIndex]->projection());          
                meshes[i]->renderObject();
            }
		}

               

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
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





