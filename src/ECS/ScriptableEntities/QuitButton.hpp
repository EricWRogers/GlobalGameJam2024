#pragma once
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>
#include <Canis/ECS/Components/ButtonComponent.hpp>

#include <Canis/AudioManager.hpp>

using namespace Canis;

class QuitButton : public Canis::ScriptableEntity
{
    static void OnClickQuitButton(void *instance)
    {
        Canis::AudioManager::Play("assets/audio/sounds/click1.wav", 0.4f);

        exit(0);
    }
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {
        Canis::Entity mainMenuButton = CreateEntity();
        auto& playRect = mainMenuButton.AddComponent<Canis::RectTransformComponent>(
            true,                                                // active
            Canis::RectAnchor::CENTER,
            glm::vec2(0.0f, -120.0f),                             // position
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
            "Quit", // text
            Canis::Text::CENTER
        );
        auto& playB = mainMenuButton.AddComponent<Canis::ButtonComponent>(
            OnClickQuitButton,
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

bool DecodeQuitButton(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "QuitButton")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<QuitButton>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}