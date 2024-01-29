#pragma once

class SplashLoader : public Canis::ScriptableEntity
{

private:
    float timer = 0.0f;
    bool hasCalledLoad = false;

public:
    void OnCreate() {}

    void OnReady()
    {
        if (GetScene().name == "engine_splash")
        {
            timer = 3.0f;
        }
        else if (GetScene().name == "splash_screen") 
        {
            timer = 3.5f;
        }
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        timer-=_dt;

        if (hasCalledLoad == true)
        {
            return;
        }

        // space to skip
        if (GetScene().name == "splash_screen")
        {
            if (GetInputManager().JustPressedKey(SDLK_SPACE))
            {
                timer = -1.0f;
            }
        }

        if (timer < 0.0f) //if (GetInputManager().JustPressedKey(SDLK_SPACE))
        {
            if (GetScene().name == "splash_screen")
            {
                ((Canis::SceneManager*)entity.scene->sceneManager)->Load("main_menu");
                hasCalledLoad = true;
                return;
            }

            if (GetScene().name == "engine_splash")
            {
                ((Canis::SceneManager*)entity.scene->sceneManager)->Load("3d_demo");
                hasCalledLoad = true;
                return;
            }
        }
    }
};

bool DecodeSplashLoader(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "SplashLoader")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<SplashLoader>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}