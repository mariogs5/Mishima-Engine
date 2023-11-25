#include "ComponentTexture.h"
#include "GameObject.h"
#include "Component.h"

#include <string>

#include "ModuleTexture.h"
#include "ModuleScene.h"
#include "External/ImGui/imgui.h"

ComponentTexture::ComponentTexture(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::TEXTURE;
};

void ComponentTexture::Enable() {}

void ComponentTexture::Disable() {}

void ComponentTexture::Update() {}

void ComponentTexture::EditorInspector()
{
    if (ImGui::CollapsingHeader("Component Texture"))
    {
        if (texture)
        {
            ImGui::Text("Path: %s", texture->path_);
            ImGui::Text("Image:");

            ImGui::Image(reinterpret_cast<void*>(static_cast<size_t>(texture->textID)), ImVec2(100, 100));
        }
    }
}

void ComponentTexture::SetTexture(Texture* texture)
{
    this->texture = texture;
}

Texture* ComponentTexture::GetTexture()
{
    return this->texture;
}


