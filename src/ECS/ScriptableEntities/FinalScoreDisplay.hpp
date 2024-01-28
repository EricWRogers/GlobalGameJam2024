#pragma once
#include <Canis/ECS/Components/RectTransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/TextComponent.hpp>

using namespace Canis;

class FinalScoreDisplay : public Canis::ScriptableEntity
{
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {
        std::string score = "Score: ";
        if (((Canis::SceneManager*)entity.scene->sceneManager)->message.contains("score"))
        {
            score += ((Canis::SceneManager*)entity.scene->sceneManager)->message["score"];
        }

        Canis::Entity mainMenuButton = CreateEntity();
        auto& playRect = mainMenuButton.AddComponent<Canis::RectTransformComponent>(
            true,                                                // active
            Canis::RectAnchor::CENTER,
            glm::vec2(0.0f, 100.0f),                             // position
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
            score, // text
            Canis::Text::CENTER
        );
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        
    }
};

bool DecodeFinalScoreDisplay(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "FinalScoreDisplay")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<FinalScoreDisplay>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}