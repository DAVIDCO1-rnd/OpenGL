#include <windows.h>

#include <GLAD/glad.h>
#include <GLAD/glad.c>
#include <GLFW/glfw3.h>

#include "math_library.h"
#include "model.cpp"
#include "scene.cpp"

#if BUILD_DEBUG
#include "renderer_software.h"
#else
#include "renderer_software.cpp"
#endif

#include <stdlib.h>
#include <stdio.h>

#include "shaders.h"

struct OpenGlContext {
    GLuint shader_program;
    GLuint vshader, fshader;
    
    GLuint screen_texture_handle;
    GLuint vertex_array_handle;
    GLuint vertex_buffer_handle;
    GLuint index_buffer_handle;
    
    GLuint vertex_buffer;
};

struct AppState {
    bool running = true;
    
    GLFWwindow *window;
    RenderData rdata = RenderData();
    Scene scene;
    OpenGlContext gl_context;
    
    float zoom = 1.f;
    float x_offset = 0.f;
    float y_offset = 0.f;
    double roll_time = .1f;
    int keyboard_mods = 0;
    
    HANDLE scene_mutex;
    HANDLE render_mutex;
    HANDLE render_thread_handle;
    
    bool should_render = true;
    bool should_double_buffer = true;
    
    float frames_per_second = 0;
    float ms_per_frame = 0;
};

#include <IMGUI/imgui.h>
#include <IMGUI/imgui.cpp>
#include <IMGUI/imgui_draw.cpp>
#include <IMGUI/imgui_demo.cpp>
#include <IMGUI/imgui_tables.cpp>
#include <IMGUI/imgui_widgets.cpp>
#include <IMGUI/backends/imgui_impl_glfw.h>
#include <IMGUI/backends/imgui_impl_opengl3.h>

#include "user_interface.cpp"

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureKeyboard) return;
    
    AppState *state = (AppState *)glfwGetWindowUserPointer(window);
    state->keyboard_mods = mods;
    
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch(key) {
            case GLFW_KEY_ESCAPE: {
                // glfwSetWindowShouldClose(window, GLFW_TRUE);
            } break;
            case GLFW_KEY_SPACE: {
                if(action == GLFW_PRESS) {
                    state->should_render = !state->should_render;
                }
            } break;
            case GLFW_KEY_RIGHT: { state->x_offset += .1f/state->zoom; } break;
            case GLFW_KEY_LEFT:  { state->x_offset -= .1f/state->zoom; } break;
            case GLFW_KEY_UP:    { state->y_offset += .1f/state->zoom; } break;
            case GLFW_KEY_DOWN:  { state->y_offset -= .1f/state->zoom; } break;
            case GLFW_KEY_EQUAL: { state->zoom = state->zoom * 1.025f; } break;
            case GLFW_KEY_MINUS: { state->zoom = state->zoom / 1.025f; } break;
            case GLFW_KEY_0: {
                state->zoom =     1.f;
                state->x_offset = 0.f;
                state->y_offset = 0.f;
            } break;
            case GLFW_KEY_J: { } break;
            case GLFW_KEY_ENTER: { } break;
        }
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse) return;
    
    AppState *state = (AppState *)glfwGetWindowUserPointer(window);
    if(state->keyboard_mods & GLFW_MOD_CONTROL) {
        if(yoffset > 0.f) state->zoom *= 1.025f;
        if(yoffset < 0.f) state->zoom /= 1.025f;
    } else {
        state->x_offset -= (xoffset/state->zoom)/8.f;
        state->y_offset += (yoffset/state->zoom)/8.f;
    }
}

static void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
    AppState *state = (AppState *)glfwGetWindowUserPointer(window);
    
    // TODO(NJ): find a way to interrupt the renderer thread instead of suspending 
    // the main window until it is done rendering
    DWORD wait_result = WaitForSingleObject(state->render_mutex, INFINITE);
    if(wait_result == WAIT_OBJECT_0) {
        RenderData *rdata = &state->rdata;
        rdata->rgba_buffer[0] = (Vec4 *)realloc(rdata->rgba_buffer[0], sizeof(*rdata->rgba_buffer[0]) * width * height);
        rdata->rgba_buffer[1] = (Vec4 *)realloc(rdata->rgba_buffer[1], sizeof(*rdata->rgba_buffer[1]) * width * height);
        rdata->width  = width;
        rdata->height = height;
        
        glViewport(0, 0, width, height);
        
        ReleaseMutex(state->render_mutex);
    }
}

GLboolean validate_shader_compilation(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        GLint error_length;
        const size_t max_error_length = 1024;
        GLchar error_log[max_error_length];
        glGetShaderInfoLog(shader, max_error_length, &error_length, error_log);
        printf("Shader error:\n%s\n", error_log);
        
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shader); // Don't leak the shader.
    }
    
    return success;
}

bool init_opengl_for_2d(OpenGlContext *gl_context) {
    gl_context->vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(gl_context->vshader, 1, &vshader_text, NULL);
    glCompileShader(gl_context->vshader);
    if(validate_shader_compilation(gl_context->vshader) == GL_FALSE) return false;
    
    gl_context->fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(gl_context->fshader, 1, &fshader_text, NULL);
    glCompileShader(gl_context->fshader);
    if(validate_shader_compilation(gl_context->fshader) == GL_FALSE) return false;
    
    gl_context->shader_program = glCreateProgram();
    glAttachShader(gl_context->shader_program, gl_context->vshader);
    glAttachShader(gl_context->shader_program, gl_context->fshader);
    glLinkProgram(gl_context->shader_program);
    
    glGenVertexArrays(1, &gl_context->vertex_array_handle);
    glGenBuffers(1, &gl_context->vertex_buffer_handle);
    glGenBuffers(1, &gl_context->index_buffer_handle);
    
    static const Vec2 vertices[] = {
        {  1.f,  1.f }, { 1.f, 1.f },
        {  1.f, -1.f }, { 1.f, 0.f },
        { -1.f, -1.f }, { 0.f, 0.f },
        { -1.f,  1.f }, { 0.f, 1.f },
    };
    
    static const GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    
    glBindVertexArray(gl_context->vertex_array_handle);
    glBindBuffer(GL_ARRAY_BUFFER, gl_context->vertex_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_context->index_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(Vec2), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(Vec2), (void*)(sizeof(Vec2)));
    glEnableVertexAttribArray(1);
    
    glGenTextures(1, &gl_context->screen_texture_handle);
    glBindTexture(GL_TEXTURE_2D, gl_context->screen_texture_handle);
    
    return true;
}

#if BUILD_DEBUG
struct RendererSoftwareDll {
    FILETIME last_compilation_time;
    HMODULE dll_handle = 0;
    Render_Model_Flat *render_model_flat = nullptr;
    Render_Model_Wireframe *render_model_wireframe = nullptr;
};

void maybe_reload_renderer_dll(RendererSoftwareDll *renderer, char *source_dll_name, char *temp_dll_name) {
    WIN32_FILE_ATTRIBUTE_DATA attribute_data, ignored;
    if(GetFileAttributesEx(source_dll_name, GetFileExInfoStandard, &attribute_data)) {
        if(CompareFileTime(&attribute_data.ftLastWriteTime, &renderer->last_compilation_time) != 0 &&
           !GetFileAttributesEx("Debug\\pdb_lock.tmp", GetFileExInfoStandard, &ignored)) {
            if(renderer->dll_handle) {
                FreeLibrary(renderer->dll_handle);
                renderer->render_model_flat = nullptr;
                renderer->render_model_wireframe = nullptr;
            }
            
            renderer->last_compilation_time = attribute_data.ftLastWriteTime;
            CopyFile(source_dll_name, temp_dll_name, 0);
            
            renderer->dll_handle = LoadLibraryA(temp_dll_name);
            if(renderer->dll_handle) {
                renderer->render_model_flat = (Render_Model_Flat *) GetProcAddress(renderer->dll_handle, "render_model_flat");
                renderer->render_model_wireframe = (Render_Model_Flat *) GetProcAddress(renderer->dll_handle, "render_model_wireframe");
            }
        }
    }
}
#endif

DWORD WINAPI render_thread_proc(LPVOID thread_data) {
    AppState *state = (AppState *)thread_data;
#if BUILD_DEBUG
    RendererSoftwareDll renderer = {};
#endif
    
    bool should_render = true;
    while(state->running) {
#if BUILD_DEBUG
        maybe_reload_renderer_dll(&renderer, "Debug\\renderer_software.dll", "Debug\\renderer_software_tmp.dll");
#endif
        
        DWORD wait_result = WaitForSingleObject(state->scene_mutex, INFINITE);
        if(wait_result == WAIT_OBJECT_0) {
            should_render = state->should_render;
            ReleaseMutex(state->scene_mutex);
        } else if(wait_result == WAIT_ABANDONED) {
            break;
        }
        
        LARGE_INTEGER begin_counter, end_counter;
        
        if(should_render) {
            wait_result = WaitForSingleObject(state->render_mutex, INFINITE);
            if(wait_result == WAIT_OBJECT_0) {
                QueryPerformanceCounter(&begin_counter);
                RenderData *rdata = &state->rdata;
                memset(rdata->rgba_buffer[0], 0.f, rdata->width*rdata->height*sizeof(*rdata->rgba_buffer[0]));
#if BUILD_DEBUG
                renderer.render_model_wireframe(rdata, &state->scene.model_list[0]);
                renderer.render_model_flat(rdata, &state->scene.model_list[0]);
#else
                render_model_wireframe(rdata, &state->scene.model_list[0]);
                render_model_flat(rdata, &state->scene.model_list[0]);
#endif
                QueryPerformanceCounter(&end_counter);
                
                if(state->should_double_buffer)
                    swap(&rdata->rgba_buffer[0], &rdata->rgba_buffer[1]);
                
                ReleaseMutex(state->render_mutex);
            } else if(wait_result == WAIT_ABANDONED) {
                break;
            }
            
            wait_result = WaitForSingleObject(state->scene_mutex, INFINITE);
            if(wait_result == WAIT_OBJECT_0) {
                ReleaseMutex(state->scene_mutex);
            } else if(wait_result == WAIT_ABANDONED) {
                break;
            }
        }
        
        LARGE_INTEGER performance_counter_frequency;
        QueryPerformanceFrequency(&performance_counter_frequency);
        
        s64 counter_elapsed = end_counter.QuadPart - begin_counter.QuadPart;
        state->ms_per_frame = 1000.f*((r32)counter_elapsed / (r32)performance_counter_frequency.QuadPart);
        state->frames_per_second = ((r32)performance_counter_frequency.QuadPart/(r32)counter_elapsed);
        
        // Locking framerate
        if(state->frames_per_second > 70.f) {
            Sleep((s32)((1000.f/70.f) - state->ms_per_frame));
        }
    }
#if BUILD_DEBUG
    if(renderer.dll_handle != 0) {
        FreeLibrary(renderer.dll_handle);
    }
#endif
    
    return 0;
}

int main(void) {
    if(glfwInit() != GLFW_TRUE) {
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwSetErrorCallback(error_callback);
    
    AppState _state = AppState();
    AppState *state = &_state;
    RenderData *rdata = &state->rdata;
    Scene *scene = &state->scene;
    Model dummy = load_model_file_obj("models\\monkey.obj");
    arrput(scene->model_list, dummy);
    arrput(scene->model_list, dummy);
    OpenGlContext *gl_context = &state->gl_context;
    
#if BUILD_DEBUG
    state->window = glfwCreateWindow(800, 800, "3D Renderer (Debug)", NULL, NULL);
#else
    state->window = glfwCreateWindow(800, 800, "3D Renderer (Release)", NULL, NULL);
#endif
    
    if (!state->window) {
        fprintf(stderr, "Error: Could not glfwCreateWindow failed\n");
        return 1;
    }
    
    glfwSetWindowUserPointer(state->window, state);
    glfwSetKeyCallback   (state->window, key_callback);
    glfwSetScrollCallback(state->window, scroll_callback);
    glfwSetFramebufferSizeCallback(state->window, framebuffer_resize_callback);
    glfwSetWindowSizeLimits(state->window, 20, 20, GLFW_DONT_CARE, GLFW_DONT_CARE);
    
    glfwMakeContextCurrent(state->window);
    gladLoadGL();
    glfwSwapInterval(1);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(state->window, true);
    ImGui_ImplOpenGL3_Init("#version 440");
    
    state->scene_mutex = CreateMutex(0, FALSE, 0);
    state->render_mutex = CreateMutex(0, FALSE, 0);
    state->render_thread_handle = CreateThread(0, 0, render_thread_proc, state, 0, 0); //CREATE_SUSPENDED, 0);
    
    int width, height;
    glfwGetFramebufferSize(state->window, &width, &height);
    framebuffer_resize_callback(state->window, width, height);
    
    bool success = init_opengl_for_2d(gl_context);
    if(success == false) return 1;
    
    while (!glfwWindowShouldClose(state->window))
    {
        DWORD wait_result = WaitForSingleObject(state->scene_mutex, INFINITE);
        if(wait_result != WAIT_OBJECT_0) {
            break;
        }
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gl_context->screen_texture_handle);
        
        if(state->should_double_buffer)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rdata->width, rdata->height, 0, GL_RGBA, GL_FLOAT, rdata->rgba_buffer[1]);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rdata->width, rdata->height, 0, GL_RGBA, GL_FLOAT, rdata->rgba_buffer[0]);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glUniform1i(glGetUniformLocation(gl_context->shader_program, "texture0"), 0);
        
        glUseProgram(gl_context->shader_program);
        glBindVertexArray(gl_context->vertex_array_handle);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // printf("Frame time: ~%f fps!\n", io.Framerate);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        generate_outliner(state);
        
        ReleaseMutex(state->scene_mutex);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwMakeContextCurrent(state->window);
        
        glfwSwapBuffers(state->window);
        
        glfwPollEvents();
    }
    
    DWORD wait_result = WaitForSingleObject(state->scene_mutex, INFINITE);
    if(wait_result == WAIT_OBJECT_0) {
        state->running = false;
        ReleaseMutex(state->scene_mutex);
        // TODO(nj): Figure out how to interrupt the thread instead...
        WaitForSingleObject(state->render_thread_handle, INFINITE);
    } else {
        TerminateThread(state->render_thread_handle, 0);
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glDeleteVertexArrays(1, &gl_context->vertex_array_handle);
    glDeleteBuffers(1, &gl_context->vertex_buffer_handle);
    glDeleteBuffers(1, &gl_context->index_buffer_handle);
    glfwDestroyWindow(state->window);
    
    glfwTerminate();
    return 0;
}
