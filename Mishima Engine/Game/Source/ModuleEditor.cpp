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

    mFPS.reserve(30);

	return ret;

}

void ModuleEditor::DrawEditor()
{
    //FPS
    aFPS = App->FPS();
    UpdateFPS(aFPS);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //-------------------------------------------Editor Code-----------------------------------------------------------------------------------//

    //ImGui::SeparatorText("Fullscreen:");   Titulo separador
     
    //ImGui::Text("dear imgui says hello! (%s) (%d)");   Texto

    ImGui::ShowDemoWindow();

    if (ImGui::BeginMainMenuBar()) 
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("File")) 
            {

            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Configuration", &show_FPS_window, ImGuiWindowFlags_MenuBar);

    if (ImGui::CollapsingHeader("Resources"))
    {
    /*    float samples[100];
        for (int n = 0; n < 100; n++)
            samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);*/

        ImGui::PlotHistogram("FPS", &mFPS[0], mFPS.size(), 0, "", 0.0f, 100.0f, ImVec2(300, 100));
    }

    if (ImGui::CollapsingHeader("Window")) 
    {
        ImGui::Text("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");

       /* if (ImGui::Checkbox("Fullscreen",&fullscreen)) 
        {
            App->window->SetFullscreen(fullscreen)
        }*/
    }
    ImGui::End();
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

void ModuleEditor::UpdateFPS(const float aFPS)
{
    if (mFPS.size() < 30) 
    {
        mFPS.push_back(aFPS);
    }
    else 
    {
        for (unsigned int i = 0; i < mFPS.size(); i++) 
        {
            if (i + 1 < mFPS.size()) 
            {
                float copy = mFPS[i + 1];
                mFPS[i] = copy;
            }
        }
        mFPS[mFPS.capacity() - 1] = aFPS;
    }
}
