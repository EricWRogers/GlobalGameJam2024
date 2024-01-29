#pragma once
#include <SDL_keyboard.h>
#include <string>

#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/ScriptComponent.hpp>
#include <Canis/ECS/Components/TransformComponent.hpp>

#include <Canis/Camera.hpp>

class PlayerCamera : public Canis::ScriptableEntity
{
private:
    bool m_mouseLock = false;
public:
    Canis::Entity target;
    float mouseSensitivity = 0.5f;

    void OnCreate() {}

    void OnReady()
    {
        m_mouseLock = true;
        GetWindow().MouseLock(m_mouseLock);
    }

    void OnDestroy() {}

    void OnUpdate(float _dt)
    {
        using namespace Canis;
        using namespace glm;

        if (GetInputManager().JustPressedKey(SDLK_ESCAPE))
        {
            m_mouseLock = !m_mouseLock;

            GetWindow().MouseLock(m_mouseLock);
        }

        TransformComponent& targetTransform = target.GetComponent<TransformComponent>();
        TransformComponent& transform = GetComponent<TransformComponent>();

        Entity parent = Entity(transform.parent, entity.scene);
        TransformComponent& parentTransform = parent.GetComponent<TransformComponent>();
        
        Camera& camera = GetCamera();

        SetTransformPosition(parentTransform, GetGlobalPosition(targetTransform));

        float inputX = 0.0f;

        if (GetInputManager().GetLeftStick(0).x != 0.0f)
            inputX = GetInputManager().GetLeftStick(0).x;
        else
            inputX = -GetInputManager().mouseRel.x;

        if (m_mouseLock == true)
            Canis::Rotate(parentTransform, vec3(0.0f, radians(inputX * mouseSensitivity), 0.0f));

        camera.Position = GetGlobalPosition(transform);
        camera.Front = normalize(GetGlobalPosition(targetTransform) - camera.Position);
    }
};

bool DecodePlayerCamera(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "PlayerCamera")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<PlayerCamera>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}