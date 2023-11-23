#pragma once
#include "Component.h"
#include "Globals.h"

#include "External/MathGeoLib/include/MathGeoLib.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/imgui/imgui.h"

class ComponentTransform : public Component {
public:
    void Enable() override;
    void Update() override;
    void Disable() override;
    void EditorInspector() override;

    void inline SetPosition(float3 position);
    void inline SetRotation(Quat rotation);
    void inline SetScale(float3 scale);

    float3 inline GetPosition() const 
    { 
        return this->newPosition; 
    };
    Quat inline GetRotation() const 
    { 
        return this->newRotation; 
    };
    float3 inline GetScale() const 
    { 
        return this->newScale; 
    };
    float4x4 inline GetTransformMatrix() const 
    { 
        return this->transform; 
    };

    void UpdateMatrix();
    ComponentTransform(GameObject* parent);
    ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);

public:

    float3 newScale{ 1.0f, 1.0f, 1.0f };
    Quat newRotation = Quat::FromEulerXYX(0.0f, 0.0f, 0.0f);
    float3 newPosition{ 0.0f, 0.0f, 0.0f };
    float4x4 transform;
};
