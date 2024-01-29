#ifdef _WIN32
#include <windows.h>
#endif

#include <Canis/App.hpp>
#include <Canis/ECS/Decode.hpp>
#include <Canis/Canis.hpp>
#include <Canis/SceneManager.hpp>

#include <Canis/ECS/Systems/RenderHUDSystem.hpp>
#include <Canis/ECS/Systems/RenderTextSystem.hpp>
#include <Canis/ECS/Systems/SpriteRenderer2DSystem.hpp>
#include <Canis/ECS/Systems/SpriteAnimationSystem.hpp>
#include <Canis/ECS/Systems/CollisionSystem2D.hpp>
#include <Canis/ECS/Systems/ButtonSystem.hpp>
#include <Canis/ECS/Systems/RenderMeshWithShadowSystem.hpp>
#include <Canis/ECS/Systems/UISliderSystem.hpp>

#include "ECS/ScriptableEntities/DebugCamera2D.hpp"
#include "ECS/ScriptableEntities/SpawnCube.hpp"
#include "ECS/ScriptableEntities/FlyCam.hpp"
#include "ECS/ScriptableEntities/FPSCounter.hpp"
#include "ECS/ScriptableEntities/SplashLoader.hpp"
#include "ECS/ScriptableEntities/PlayerCamera.hpp"
#include "ECS/ScriptableEntities/RetryButton.hpp"
#include "ECS/ScriptableEntities/QuitButton.hpp"
#include "ECS/ScriptableEntities/FinalScoreDisplay.hpp"
#include "ECS/ScriptableEntities/ScoreManager.hpp"

// Define these only in *one* .cc file.
    #define TINYGLTF_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    // #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
    #include <Canis/External/TinyGLTF.hpp>

int main(int argc, char* argv[])
{
    

    using namespace tinygltf;

    Model model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    //bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, "assets/models/shark.gltf");
    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, "assets/models/shark.glb"); // for binary glTF(.glb)

    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
        return -1;
    }

    for (int i = 0; i < model.meshes.size(); i++)
    {
        std::cout << model.meshes[i].name << std::endl;
    }

    Canis::App app;

    // decode system
    app.AddDecodeSystem(Canis::DecodeButtonSystem);
    app.AddDecodeSystem(Canis::DecodeCollisionSystem2D);
	app.AddDecodeSystem(Canis::DecodeSpriteAnimationSystem);
    app.AddDecodeSystem(Canis::DecodeUISliderSystem);

    // decode render system
    app.AddDecodeRenderSystem(Canis::DecodeRenderHUDSystem);
    app.AddDecodeRenderSystem(Canis::DecodeRenderTextSystem);
    app.AddDecodeRenderSystem(Canis::DecodeSpriteRenderer2DSystem);
    app.AddDecodeRenderSystem(Canis::DecodeRenderMeshWithShadowSystem);

    // decode scriptable entities
    app.AddDecodeScriptableEntity(DecodeDebugCamera2D);
    app.AddDecodeScriptableEntity(DecodeSpawnCube);
    app.AddDecodeScriptableEntity(DecodeFlyCam);
    app.AddDecodeScriptableEntity(DecodeFPSCounter);
    app.AddDecodeScriptableEntity(DecodeSplashLoader);
    app.AddDecodeScriptableEntity(DecodeRetryButton);
    app.AddDecodeScriptableEntity(DecodeQuitButton);
    app.AddDecodeScriptableEntity(DecodeFinalScoreDisplay);
    app.AddDecodeScriptableEntity(DecodeScoreManager);

    // decode component
    app.AddDecodeComponent(Canis::DecodeTransformComponent);
    app.AddDecodeComponent(Canis::DecodeMeshComponent);
    app.AddDecodeComponent(Canis::DecodeSphereColliderComponent);
    app.AddDecodeComponent(Canis::DecodeDirectionalLightComponent);
    app.AddDecodeComponent(Canis::DecodeTagComponent);
    app.AddDecodeComponent(Canis::DecodeCamera2DComponent);
    app.AddDecodeComponent(Canis::DecodeRectTransformComponent);
    app.AddDecodeComponent(Canis::DecodeColorComponent);
    app.AddDecodeComponent(Canis::DecodeTextComponent);
    app.AddDecodeComponent(Canis::DecodeSprite2DComponent);
    app.AddDecodeComponent(Canis::DecodeUIImageComponent);
    app.AddDecodeComponent(Canis::DecodeUISliderComponent);
    app.AddDecodeComponent(Canis::DecodeSpriteAnimationComponent);
    app.AddDecodeComponent(Canis::DecodeCircleColliderComponent);

    app.AddSplashScene(new Canis::Scene("engine_splash", "assets/scenes/engine_splash.scene"));
    app.AddScene(new Canis::Scene("3d_demo", "assets/scenes/3d_demo.scene"));
    app.AddScene(new Canis::Scene("game_over", "assets/scenes/game_over.scene"));

    app.Run("Shark Bonk", "engine_splash");

    return 0;
}