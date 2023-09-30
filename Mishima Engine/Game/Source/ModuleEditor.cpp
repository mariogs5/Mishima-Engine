#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "Application.h"

#include <iostream>
#include "vector"


ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	bool ret = true;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

        UpdateAditionalWindows = true;
    }

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    vectorFPS.reserve(30); //FPS vector
    vectorMS.reserve(30); //MS vector
    vectorDT.reserve(30); //DT vector

	return ret;

}

void ModuleEditor::DrawEditor()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //-------------------------------------------Editor Code-----------------------------------------------------------------------------------//

    //ImGui::SeparatorText("Fullscreen:");   Titulo separador
     
    //ImGui::Text("dear imgui says hello! (%s) (%d)");   Texto

    //ImGui::ShowDemoWindow();

    MainMenuBar();
    if (configWindow) 
    {
        ConfigurationWindow();
    }

    //-------------------------------------------End of Editor Code---------------------------------------------------------------------------//

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (UpdateAditionalWindows)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

bool ModuleEditor::CleanUp()
{	
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}

// imgui Funcions
void ModuleEditor::MainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Configuration"))
            {
                configWindow = true;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ModuleEditor :: ConfigurationWindow()
{
    //Update current FPS and vector
    currentFPS = App->FPS();
    UpdateFPS(currentFPS);

    //Update current MS and vector
    currentMS = App->MS();
    UpdateMS(currentMS);

    //Update current DT and vector
    currentDT = App->DT();
    UpdateDT(currentDT);

    ImGui::Begin("Configuration", &configWindow, ImGuiWindowFlags_MenuBar);

    if (ImGui::CollapsingHeader("Resources"))
    {
        ImGui::PlotHistogram("", &vectorFPS[0], vectorFPS.size(), 0, "Framerate", 0.0f, 200.0f, ImVec2(300, 80));
        ImGui::PlotHistogram("", &vectorMS[0], vectorMS.size(), 0, "Milliseconds", 0.0f, 10.0f, ImVec2(300, 80));
        ImGui::PlotHistogram("", &vectorDT[0], vectorDT.size(), 0, "Delta Time", 0.0f, 0.01f, ImVec2(300, 80));
    }

    if (ImGui::CollapsingHeader("Window"))
    {
         if (ImGui::Checkbox("Fullscreen",&fullscreen))
         {
             App->window->EnableFullscreen(fullscreen);
         }
    }

    if (ImGui::CollapsingHeader("Hardware & Drivers"))
    {
        ImGui::BulletText("Vendor: % s", glGetString(GL_VENDOR));
        ImGui::BulletText("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::BulletText("OpenGL version supported %s", glGetString(GL_VERSION));
        ImGui::BulletText("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::BulletText("Using Glew %s", glewGetString(GLEW_VERSION));
    }

    if (ImGui::CollapsingHeader("About"))
    {
        //Name of the engine
        //Name of authors with link to github
        //Libraries used in real time with link to the web
        //Text of the license 

    }

    ImGui::End();
}


// Resources Funcions
void ModuleEditor::UpdateFPS(const float currentFPS)
{
    if (vectorFPS.size() < 30)
    {
        vectorFPS.push_back(currentFPS);
    }
    else 
    {
        for (unsigned int i = 0; i < vectorFPS.size(); i++)
        {
            if (i + 1 < vectorFPS.size())
            {
                float copy = vectorFPS[i + 1];
                vectorFPS[i] = copy;
            }
        }
        vectorFPS[vectorFPS.capacity() - 1] = currentFPS;
    }
}

void ModuleEditor::UpdateMS(const float currentMS) 
{
    if (vectorMS.size() < 30)
    {
        vectorMS.push_back(currentMS);
    }
    else
    {
        for (unsigned int i = 0; i < vectorMS.size(); i++)
        {
            if (i + 1 < vectorMS.size())
            {
                float copy = vectorMS[i + 1];
                vectorMS[i] = copy;
            }
        }
        vectorMS[vectorMS.capacity() - 1] = currentMS;
    }
}

void ModuleEditor::UpdateDT(const float currentDT)
{
    if (vectorDT.size() < 30)
    {
        vectorDT.push_back(currentDT);
    }
    else
    {
        for (unsigned int i = 0; i < vectorDT.size(); i++)
        {
            if (i + 1 < vectorDT.size())
            {
                float copy = vectorDT[i + 1];
                vectorDT[i] = copy;
            }
        }
        vectorDT[vectorDT.capacity() - 1] = currentDT;
    }
}
