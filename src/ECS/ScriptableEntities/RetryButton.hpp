#pragma once
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>
#include <Canis/ECS/Components/ButtonComponent.hpp>

#include <Canis/AudioManager.hpp>

using namespace Canis;

class RetryButton : public Canis::ScriptableEntity
{
    static void OnClickMainMenu(void *instance)
    {
        Canis::AudioManager::Play("assets/audio/sounds/click1.wav", 0.4f);

        ((Canis::SceneManager *)((RetryButton * )instance)->entity.scene->sceneManager)->Load("3d_demo");
    }
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {
        GetWindow().MouseLock(false);

        Canis::Entity mainMenuButton = CreateEntity();
        auto& playRect = mainMenuButton.AddComponent<Canis::RectTransformComponent>(
            true,                                                // active
            Canis::RectAnchor::CENTER,
            glm::vec2(0.0f, -50.0f),                             // position
            glm::vec2(150.0f, 40.0f),                            // size
            glm::vec2(0.0f, 0.0f),                               // originOffset
            0.0f,                                                // rotation
            1.0f,                                                // scale
            0.0f 
        );
        auto& playColor = mainMenuButton.AddComponent<Canis::ColorComponent>(
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
        );
        auto& playText = mainMenuButton.AddComponent<Canis::TextComponent>(
            AssetManager::LoadText("assets/fonts/Antonio-Bold.ttf", 48),
            "Play Again", // text
            Canis::Text::CENTER
        );
        auto& playB = mainMenuButton.AddComponent<Canis::ButtonComponent>(
            OnClickMainMenu,
            this,
            glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
        );
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        
    }
};

bool DecodeRetryButton(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "RetryButton")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<RetryButton>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}