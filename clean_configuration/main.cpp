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
#include "Shaders/shader_m.h"

#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
//#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <string>
#include "Mesh.h"

using namespace std;

void printOpenGLError(GLenum err)
{
	switch (err)
	{
	case GL_INVALID_ENUM:
		cout << "Error: GL_INVALID_ENUM" << endl;
		system("pause");
		break;
	case GL_INVALID_VALUE:
		cout << "Error: GL_INVALID_VALUE" << endl;
		system("pause");
		break;
	case GL_INVALID_OPERATION:
		cout << "Error: GL_INVALID_OPERATION" << endl;
		system("pause");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		cout << "Error: GL_INVALID_FRAMEBUFFER_OPERATION" << endl;
		system("pause");
		break;
	case GL_OUT_OF_MEMORY:
		cout << "Error: GL_OUT_OF_MEMORY" << endl;
		system("pause");
		break;
	case GL_NO_ERROR:
		cout << "No error reported" << endl;
		break;
	default:
		cout << "Unknown error" << endl;
		break;
	}
}

struct Parameters
{
	GLfloat angleX = 0.0f;
	GLfloat angleY = 0.0f;
	GLfloat angleZ = 0.0f;
	GLfloat scaleX = 0.25f;
	GLfloat scaleY = 0.25f;
	GLfloat scaleZ = 0.25f;
	GLfloat scaleUniform = 0.25f; //for uniform scaling (in all the axises)
	GLfloat translateX = 0.0f;
	GLfloat translateY = 0.0f;
	GLfloat translateZ = 0.0f;
};

Parameters params;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

/*Updates the modelView matrix by the user parameters*/
void updateModelViewByUserParameters(glm::mat4 &model)
{
	model = glm::rotate(model, glm::radians(params.angleX), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(params.angleY), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(params.angleZ), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(params.translateX, params.translateY, params.translateZ));
	model = glm::scale(model, glm::vec3(params.scaleX, params.scaleY, params.scaleZ));
	model = glm::scale(model, glm::vec3(params.scaleUniform, params.scaleUniform, params.scaleUniform));
}

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

void renderObject(Mesh* mesh)
{
	GLenum err011 = glGetError();
	if (err011 != GL_NO_ERROR)
	{
		cout << "err011" << endl;
		printOpenGLError(err011);
	}
	glBindVertexArray(mesh->VAO);
	GLenum err012 = glGetError();
	if (err012 != GL_NO_ERROR)
	{
		cout << "err012" << endl;
		printOpenGLError(err012);
	}

	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
	GLenum err013 = glGetError();
	if (err013 != GL_NO_ERROR)
	{
		cout << "err013" << endl;
		printOpenGLError(err013);
	}

	glBindVertexArray(0);
	GLenum err014 = glGetError();
	if (err014 != GL_NO_ERROR)
	{
		cout << "err014" << endl;
		printOpenGLError(err014);
	}
}

int main(int, char**)
{
    string fileName = "bunny";
	string filePath = "/home/davidco1/Developments/OpenGL/clean_configuration/Data/" + fileName + ".obj";
    Mesh myMesh(filePath);
	float signedDistOfPlaneFromOrigin = -4.0f;
	int width = 600;
	int height = 800;
	int gridX = 600;
	int gridY = 800;
	bool renderMesh = true;
	bool renderIntersection = true;

    GLfloat planeAngleX = 0.0f;
	GLfloat planeAngleY = 0.0f;
	GLfloat planeAngleZ = 0.0f;

	GLfloat lastPlaneRotationX = 0.0f;
	GLfloat lastPlaneRotationY = 0.0f;
	GLfloat lastPlaneRotationZ = 0.0f;

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    int glfwInitStatus = glfwInit();
    if (glfwInitStatus == 0)
    {
        return 1;
    }
    // if (!glfwInit())
    //     return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

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
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Shader shaderWithoutFilters("/home/davidco1/Developments/OpenGL/clean_configuration/resources/shaders/shaderWithoutFilters.vs", "/home/davidco1/Developments/OpenGL/clean_configuration/resources/shaders/shaderWithoutFilters.fs");

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

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            
            ImGui::Begin("Hello, Rafael!");
            {
                static float f = 0.0f;
                static int counter = 0;
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
                {
                    //ImGui::InputFloat("rotate X", &params.angleX, 1.0f, 1.0f);
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
                    //float planeNormalArray[3] = { rotatedPlaneNormal[0], rotatedPlaneNormal[1], rotatedPlaneNormal[2] };
                    //ImGui::InputFloat3("Plane Normal", planeNormalArray);
                }
            }           

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);


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
