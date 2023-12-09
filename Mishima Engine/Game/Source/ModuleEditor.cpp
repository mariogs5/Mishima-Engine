#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "Application.h"
#include <iostream>
#include <fstream>
#include <string>
#include "External/SDL/include/SDL.h"
#include "External/Assimp/include/version.h"
#include "External/DevIL/include/il.h"

#include "GameObject.h"


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

    //Prueba crear GameObject
    //App->scene->CreateGameObject("juan");

	return ret;

}

void ModuleEditor::DrawEditor()
{
    //Toggle Fullscreen
    if (fullscreen) 
    {
        SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else 
    {
        SDL_SetWindowFullscreen(App->window->window, 0);
    }
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //-------------------------------------------Editor Code-----------------------------------------------------------------------------------//

    ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);
    
    //ImGui::SeparatorText("Fullscreen:");   Titulo separador
     
    //ImGui::Text("dear imgui says hello! (%s) (%d)");   Texto

    //ImGui::ShowDemoWindow();

    MainMenuBar();

    if (consoleWindow) 
    {
        Console();
    }

    if (configWindow) 
    {
        ConfigurationWindow();
    }

    if (inspectorWindow) 
    {
        InspectorWindow();
    }


    if (hierarchyWindow) 
    {
        ImGui::Begin("Hierarchy");

        DrawHierarchy();

        ImGui::End();
    }

    if (sceneWindow) {

        SceneWindow();
    }

    if (gameWindow) {

        GameWindow();
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

void ModuleEditor::SceneWindow()
{
    ImGui::Begin("Scene", &sceneWindow);

    ImVec2 size = ImGui::GetContentRegionAvail();
    App->camera->EditorCamera->SetAspectRatio(size.x / size.y);
    ImGui::Image((ImTextureID)App->camera->EditorCamera->TCB, size, ImVec2(0, 1), ImVec2(1, 0));

    ImVec2 MouseWindowPosition = ImGui::GetMousePos();
    ImVec2 SceneWindowPos = ImGui::GetWindowPos();
    ImVec2 SceneWindowSize = ImGui::GetWindowSize();
    ImVec2 SceneContentRegionMax = ImGui::GetContentRegionMax();

    float sceneFrameHeightOffset = ImGui::GetFrameHeight() / 2.0f;

    if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
    {
        ImVec2 normalizedPoint = NormalizeWindowPickingPoint(SceneWindowPos.x, SceneWindowPos.y + (sceneFrameHeightOffset * 2), SceneWindowSize.x, SceneWindowSize.y - (sceneFrameHeightOffset * 2), MouseWindowPosition);

        normalizedPoint.x = (normalizedPoint.x - 0.5f) / 0.5f;
        normalizedPoint.y = -((normalizedPoint.y - 0.5f) / 0.5f);

        if ((normalizedPoint.x >= -1 && normalizedPoint.x <= 1) && (normalizedPoint.y >= -1 && normalizedPoint.y <= 1))
        {
            App->camera->MousePicking(normalizedPoint.x, normalizedPoint.y);
        }
    }

    ImGui::End();
}
void ModuleEditor::GameWindow()
{
    ImGui::Begin("Game", &gameWindow);

    ImVec2 size = ImGui::GetContentRegionAvail();
    App->scene->gameCameraComponent->SetAspectRatio(size.x / size.y);
    ImGui::Image((ImTextureID)App->scene->gameCameraComponent->TCB, size, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

void ModuleEditor::MainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("Quit"))
            {
                App->input->quit = true;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Configuration"))
            {
                configWindow = true;
            }

            if (ImGui::MenuItem("Console"))
            {
                consoleWindow = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Models"))
        {
            if (ImGui::BeginMenu("Primitives")) 
            {
                ImGui::BulletText("Create Primitives");

                if (ImGui::MenuItem("Cube"))
                {
                    App->renderer3D->primCube = true;
                }
                if (ImGui::MenuItem("Sphere"))
                {
                    App->renderer3D->primSphere = true;
                }
                if (ImGui::MenuItem("Cylinder"))
                {
                    App->renderer3D->primCylinder = true;
                }
                if (ImGui::MenuItem("Cone"))
                {
                    App->renderer3D->primCone = true;
                }
                if (ImGui::MenuItem("Torus"))
                {
                    App->renderer3D->primTorus = true;
                }

                ImGui::BulletText("Engine Models");
                if (ImGui::MenuItem("Baker House"))
                {
                    App->renderer3D->primBakerHouse = true;
                }
                if (ImGui::MenuItem("Aranara"))
                {
                    App->renderer3D->primAranara = true;
                }
                if (ImGui::MenuItem("Zhongli"))
                {
                    App->renderer3D->primZhongli = true;
                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Delete Models"))
            {
                //App->renderer3D->Models.clear();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ModuleEditor::InspectorWindow()
{
    if (inspectorWindow)
    {
        ImGui::Begin("Inspector", &inspectorWindow);

        if (GameObject_selected != nullptr)
        {
            ImGui::Checkbox("Active", &GameObject_selected->active);
            ImGui::SameLine;
            strcpy(newName, GameObject_selected->name.c_str());
            if (ImGui::InputText(" ", &newName[0], sizeof(newName)))
            {
                GameObject_selected->name = newName;
            }

            if (ImGui::Button("Delete")) {

                GameObject_selected->deleteGameObject = true;
                delete GameObject_selected;
            }

            ImGui::Separator();

            for (uint m = 0; m < GameObject_selected->components.size(); m++)
            {
                GameObject_selected->components[m]->EditorInspector();
            }

            ImGui::Dummy(ImVec2(0, 15));
            ImGui::Text(" ");
            ImGui::SameLine();
            // Inicia el combo
            if (ImGui::BeginCombo(" ", "AddComponent"))
            {
                showAddComponent = true;
                if (showAddComponent)
                {//Pregunta profe
                    if (ImGui::Selectable("ComponentTexture"))
                    {
                        if (GameObject_selected->GetComponent(ComponentTypes::TEXTURE) == nullptr)
                        {
                            GameObject_selected->AddComponent(ComponentTypes::TEXTURE);
                        }
                    }
                }
                // Finaliza el combo
                ImGui::EndCombo();
            }
        }

        ImGui::End();
    }
}

void ModuleEditor::DrawHierarchyLevel(GameObject* currentObject, int num)
{
    ImGuiTreeNodeFlags flag_TNode = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow;

    bool isNodeOpen;
    bool isSelected = GameObject_selected == currentObject;

    if (currentObject->Children.size() != 0) {
        isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)num, flag_TNode, currentObject->name.c_str(), num);
    }

    else {
        flag_TNode |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)num, flag_TNode, currentObject->name.c_str(), num);
        isNodeOpen = false;
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObject", currentObject, sizeof(GameObject*));
        draggedGameObject = currentObject;
        ImGui::Text("Dragging GameObject");
        ImGui::EndDragDropSource();
    }

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
    {
        hoveredGameObj = currentObject;
        if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            GameObject_selected = currentObject;
        }
    }

    if (ImGui::IsWindowHovered())
    {
        if (!ImGui::IsAnyItemHovered())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                GameObject_selected = nullptr;
            }
        }
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {

            draggedGameObject->SetParent(hoveredGameObj);

        }
        ImGui::EndDragDropTarget();
    }

    if (isNodeOpen)
    {
        if (!currentObject->Children.empty()) {
            for (unsigned int i = 0; i < currentObject->Children.size(); i++)
            {
                DrawHierarchyLevel(currentObject->Children[i], i);
            }
        }
        ImGui::TreePop();
    }

}

void ModuleEditor::DrawHierarchy()
{
    std::vector<GameObject*> lista_games = App->scene->rootObject->Children;

    for (uint i = 0; i < lista_games.size(); i++)
    {
        DrawHierarchyLevel(lista_games[i], i);
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

    ImGui::Begin("Configuration", &configWindow);

    if (ImGui::CollapsingHeader("Resources"))
    {
        ImGui::PlotHistogram("", &vectorFPS[0], vectorFPS.size(), 0, "Framerate", 0.0f, 200.0f, ImVec2(300, 80));
        ImGui::PlotHistogram("", &vectorMS[0], vectorMS.size(), 0, "Milliseconds", 0.0f, 10.0f, ImVec2(300, 80));
        ImGui::PlotHistogram("", &vectorDT[0], vectorDT.size(), 0, "Delta Time", 0.0f, 0.01f, ImVec2(300, 80));
    }

    if (ImGui::CollapsingHeader("Window"))
    {
        ImGui::Checkbox("Fullscreen", &fullscreen);
        ImGui::SliderInt("Window Width", &App->window->width, 100, 1920);
        ImGui::SliderInt("Window Height", &App->window->height, 100, 1080);

        SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
    }

    if (ImGui::CollapsingHeader("Hardware"))
    {
        //get hardware usage, cache, count...
        int CPUCache;
        int CPUCount;
        int RAMUsage;
        GLint totalMemory;
        GLint availableMemory;
        GLint evictedMemory;

        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemory);
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableMemory);
        glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &evictedMemory);
        CPUCount = SDL_GetCPUCount();
        CPUCache = SDL_GetCPUCacheLineSize();
        RAMUsage = SDL_GetSystemRAM();
        
        ImGui::SeparatorText("HARDWARE IN USE");
        ImGui::BulletText("Vendor: % s", glGetString(GL_VENDOR));
        ImGui::BulletText("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::BulletText("VRAM Budget: %d MB", totalMemory / 1000);
        ImGui::BulletText("VRAM Available: %d MB", availableMemory / 1000);
        ImGui::BulletText("VRAM Usage: %d MB", evictedMemory / 1000);
        ImGui::BulletText("CPU Cache in use: %d in bytes", CPUCache);
        ImGui::BulletText("CPU cores: %d", CPUCount);
        ImGui::BulletText("RAM in use: %d in mb", RAMUsage / 100);
    }

    if (ImGui::CollapsingHeader("About"))
    {
        
        //Name of the engine
        ImGui::BulletText("Mishima Engine");
        ImGui::Text("A 3D Game Engine that works somehow");
        //Name of authors with link to github
        ImGui::SeparatorText("AUTHORS");

        ImGui::BulletText("Mario Garcia: ");
        ImGui::SameLine(); ImGui::Text("[github]");
        if (ImGui::IsItemClicked())
        {
            BrowserLink("https://github.com/Mariogs5");
        }
        ImGui::BulletText("Victor Gil: ");
        ImGui::SameLine(); ImGui::Text("[github]");
        if (ImGui::IsItemClicked()) 
        {
            BrowserLink("https://github.com/Ludo-pixel");
        }

        //get libraries versions
        SDL_version sdlVersion;
        SDL_GetVersion(&sdlVersion);

        int AiMajor = aiGetVersionMajor();
        int AiMinor = aiGetVersionMinor();
        int AiPatch = aiGetVersionRevision();

        ILuint version = ilGetInteger(IL_VERSION_NUM);
        int DevILpatch = version;

        ImGui::SeparatorText("LIBRARIES");
        ImGui::BulletText("OpenGL version supported %s", glGetString(GL_VERSION));
        ImGui::BulletText("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::BulletText("Using Glew %s", glewGetString(GLEW_VERSION));
        ImGui::BulletText("Using SDL %d.%d.%d", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
        ImGui::BulletText("Using Assimp %d.%d.%d", AiMajor, AiMinor, AiPatch);
        ImGui::BulletText("Using DevIL %d", DevILpatch);
        ImGui::BulletText("Using ImGui %s", IMGUI_VERSION);
        ImGui::BulletText("Using MathGeoLib");
       
        License = ReadMyFile("../../LICENSE");
        ImGui::SeparatorText("LICENSE");
        ImGui::TextWrapped("%s", License.c_str());
    }

    if (ImGui::CollapsingHeader("Open GL settings"))
    {
        ImGui::SeparatorText("Enable / Disable Open GL settings");

        if (ImGui::BeginTable("", 2))
        {
            ImGui::TableNextColumn();
            if (ImGui::Checkbox("Lighting calculations", &lightCalc))
            {
                if (lightCalc)
                {
                    glEnable(GL_LIGHTING);
                }
                else
                {
                    glDisable(GL_LIGHTING);
                }
            }

            ImGui::TableNextColumn(); 
            if (ImGui::Checkbox("Normalize Normals", &normalize))
            {
                if (normalize)
                {
                    glEnable(GL_NORMALIZE);
                }
                else
                {
                    glDisable(GL_NORMALIZE);
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Checkbox("Backface Culling", &culling))
            {
                if (culling)
                {
                    glEnable(GL_CULL_FACE);
                }
                else
                {
                    glDisable(GL_CULL_FACE);
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Checkbox("Depth Test", &depthTest))
            {
                if (depthTest)
                {
                    glEnable(GL_DEPTH_TEST);
                }
                else
                {
                    glDisable(GL_DEPTH_TEST);
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Checkbox("2D tecture mapping", &textureMapping))
            {
                if (textureMapping)
                {
                    glEnable(GL_TEXTURE_2D);
                }
                else
                {
                    glDisable(GL_TEXTURE_2D);
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Checkbox("Color material", &colorMaterial))
            {
                if (colorMaterial)
                {
                    glEnable(GL_COLOR_MATERIAL);
                }
                else
                {
                    glDisable(GL_COLOR_MATERIAL);
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Checkbox("Alpha blendig", &blend))
            {
                if (blend)
                {
                    glEnable(GL_BLEND);
                }
                else
                {
                    glDisable(GL_BLEND);
                }
            }

            ImGui::TableNextColumn();
            if (ImGui::Checkbox("Wireframe", &wireframe))
            {
                if (wireframe)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }

            ImGui::EndTable();
        }
    }

    ImGui::End();
}

void ModuleEditor::Console()
{
    ImGui::Begin("Console", &consoleWindow);

    for (int i = 0; i < consoleText.size(); i++)
    {
        ImGui::Text(consoleText[i].c_str());
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

void ModuleEditor::BrowserLink(const char* url)
{
    HINSTANCE result = ShellExecute(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
}

std::string ModuleEditor::ReadMyFile(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return "Error: Unable to open file.";
    }

    std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    return fileContents;
}

ImVec2 ModuleEditor::NormalizeWindowPickingPoint(const float& x, const float& y, const float& w, const float& h, const ImVec2& clickPoint)
{
    ImVec2 normalizedWindowPickingPoint;
    normalizedWindowPickingPoint.x = (clickPoint.x - x) / ((x + w) - x);
    normalizedWindowPickingPoint.y = (clickPoint.y - y) / ((y + h) - y);

    return normalizedWindowPickingPoint;
}
