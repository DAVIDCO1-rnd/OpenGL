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
#include "Shaders/shader_t.h"

#include <opencv2/opencv.hpp>
#include "ModelParameters.h"

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



int main(int, char**)
{


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

    Shader shaderWithoutFilters("/home/dell/Developments/OpenGL/clean_configuration/resources/shaders/shaderWithoutFilters.vs", "/home/dell/Developments/OpenGL/clean_configuration/resources/shaders/shaderWithoutFilters.fs");
	Shader shaderRed("/home/dell/Developments/OpenGL/clean_configuration/resources/shaders/shaderRed.vs", "/home/dell/Developments/OpenGL/clean_configuration/resources/shaders/shaderRed.fs");
	Shader shaderAxis("/home/dell/Developments/OpenGL/clean_configuration/resources/shaders/shaderAxis.vs", "/home/dell/Developments/OpenGL/clean_configuration/resources/shaders/shaderAxis.fs");

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    
    

    // fs::path modelsFolder1 ("/home/dell/Developments/OpenGL/clean_configuration/Data");
    // fs::path modelName1 ("teapot.obj");
    // fs::path modelFullPath1 = modelsFolder1 / modelName1;
    // cout << modelFullPath1 << std::endl;


    string modelsFolder = "/home/dell/Developments/OpenGL/clean_configuration/Data/"; 

    string modelName1 = "teapot";
    string filePath1 = modelsFolder + modelName1 + ".obj";
    ModelParameters meshParams1;
    Mesh mesh1(modelName1, filePath1, meshParams1); 
    mesh1.saveBuffersForRedneringWholeMesh();  

    string modelName2 = "bunny";
    string filePath2 = modelsFolder + modelName2 + ".obj";
    ModelParameters meshParams2;
    meshParams2.scaleUniform = 0.02f;
    Mesh mesh2(modelName2, filePath2, meshParams2); 
    mesh2.saveBuffersForRedneringWholeMesh();      

    std::vector<Mesh> meshes;
    meshes.push_back(mesh1);  
    meshes.push_back(mesh2);

    

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);



        static size_t item_current_idx = 0; // Here we store our selection data as an index.

        string modelName = meshes[item_current_idx].getModelName();
        ImGui::Begin(modelName.c_str());
        {
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            vector<string> items;
            for (size_t i=0 ; i<meshes.size() ; i++)
            {
                items.push_back(meshes[i].getModelName());
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
                ModelParameters meshParams = meshes[item_current_idx].getParams();

                meshParams.angleX = meshes[item_current_idx].getParams().angleX;
                meshParams.angleY = meshes[item_current_idx].getParams().angleY;
                meshParams.angleZ = meshes[item_current_idx].getParams().angleZ;

                meshParams.translateX = meshes[item_current_idx].getParams().translateX;
                meshParams.translateY = meshes[item_current_idx].getParams().translateY;
                meshParams.translateZ = meshes[item_current_idx].getParams().translateZ;

                meshParams.scaleX = meshes[item_current_idx].getParams().scaleX;
                meshParams.scaleY = meshes[item_current_idx].getParams().scaleY;
                meshParams.scaleZ = meshes[item_current_idx].getParams().scaleZ;

                meshParams.scaleUniform = meshes[item_current_idx].getParams().scaleUniform;

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

                meshes[item_current_idx].setParams(meshParams);                
            }
        }           

        // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //     counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

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


        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);      
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 view, projection;
        view = glm::mat4(1.0);
        projection = glm::mat4(1.0);
        for (size_t i = 0 ; i < meshes.size() ; i++) {
            meshes[i].updateModelMatrixByUserParameters();
        }		
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		if (renderMesh)
		{
			glLineWidth(1.0);             
			shaderWithoutFilters.use();      
            for (size_t i = 0 ; i < meshes.size() ; i++) {
                sendTransformationToVertexShader(shaderWithoutFilters, meshes[i].getModelMatrix(), view, projection);          
                meshes[i].renderObject();
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


