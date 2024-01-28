#pragma once
#include <SDL_keyboard.h>
#include <string>

#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/ScriptComponent.hpp>
#include <Canis/ECS/Components/TransformComponent.hpp>

#include <Canis/Camera.hpp>
#include <Canis/SceneManager.hpp>

class ScoreManager : public Canis::ScriptableEntity
{
private:
    bool m_mouseLock = false;
public:
    Canis::Entity target;
    float mouseSensitivity = 0.5f;
    float score;

    void OnCreate() {}

    void OnReady()
    {
        m_mouseLock = true;
        GetWindow().MouseLock(m_mouseLock);
    }

    void OnDestroy() {
        ((Canis::SceneManager *)entity.scene->sceneManager)->nextMessage["score"] = std::to_string((int)score);
    }

    void OnUpdate(float _dt)
    {
        using namespace Canis;
        using namespace glm;

        score += _dt;
    }
};

bool DecodeScoreManager(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "ScoreManager")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<ScoreManager>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}