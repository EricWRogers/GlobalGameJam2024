#pragma once
#include <SDL_keyboard.h>
#include <string>

#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/TransformComponent.hpp>
#include <Canis/ECS/Components/ColorComponent.hpp>
#include <Canis/ECS/Components/MeshComponent.hpp>
#include <Canis/ECS/Components/SphereColliderComponent.hpp>

#include "Rotate.hpp"
#include "LookAtTarget.hpp"
#include "PlayerCamera.hpp"
#include "PlayerMovement.hpp"
#include "Turtle.hpp"
#include "Chicken.hpp"
#include "Shark.hpp"

class SpawnCube : public Canis::ScriptableEntity
{
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {      

        Canis::TransformComponent transform;
        transform.registry = &(GetScene().entityRegistry);
        Canis::SetTransformScale(transform, glm::vec3(5.0f, 1.5f, 5.0f));

        Canis::MeshComponent mesh;
        mesh.id = Canis::AssetManager::LoadModel("assets/models/raft.obj");
        mesh.material = Canis::AssetManager::LoadMaterial("assets/materials/yes_default.material");

        Canis::SphereColliderComponent collider;

        Canis::Entity cube = CreateEntity();
        cube.AddComponent<Canis::TransformComponent>(transform);
        cube.AddComponent<Canis::ColorComponent>();
        cube.AddComponent<Canis::MeshComponent>(mesh);
        cube.AddComponent<Canis::SphereColliderComponent>(collider);

        mesh.id = Canis::AssetManager::LoadModel("assets/models/white_block.obj");
        mesh.material = Canis::AssetManager::LoadMaterial("assets/materials/box.material");



        Canis::Entity monkey = CreateEntity();

        {
            Canis::TransformComponent tr;
            tr.registry = &(GetScene().entityRegistry);
            SetTransformPosition(tr, glm::vec3(0.0f, 3.0f, 0.0f));  
            SetTransformScale(tr, glm::vec3(1.0f, 1.0f, 1.0f));

            mesh.id = Canis::AssetManager::LoadModel("assets/models/pogo.obj");        

            monkey.AddComponent<Canis::TransformComponent>(tr);
            monkey.AddComponent<Canis::ColorComponent>();
            monkey.AddComponent<Canis::MeshComponent>(mesh);
            monkey.AddComponent<Canis::SphereColliderComponent>(collider);

            Canis::SetTransformPosition(tr, glm::vec3(0.0f, 0.5f, -0.5f));
            
            GetWindow().MouseLock(false);
            mesh.id = Canis::AssetManager::LoadModel("assets/models/CesiumMan.gltf");
            Canis::Entity humanGraphics = CreateEntity();
            Canis::TransformComponent &tc = humanGraphics.AddComponent<Canis::TransformComponent>(transform);
            humanGraphics.AddComponent<Canis::ColorComponent>();
            humanGraphics.AddComponent<Canis::MeshComponent>(mesh);
            humanGraphics.AddComponent<Canis::SphereColliderComponent>(collider);
            humanGraphics.SetParent(monkey);
            Canis::SetTransformScale(tc, glm::vec3(0.7f));
        }

        PlayerMovement& playerMovement = monkey.AddScript<PlayerMovement>();

        {
            Canis::Entity cameraPivot = CreateEntity();
            Canis::TransformComponent& camPivotTransform = cameraPivot.AddComponent<Canis::TransformComponent>();
            camPivotTransform.registry = &(GetScene().entityRegistry); 

            Canis::Entity camera = CreateEntity();
            Canis::TransformComponent& camTransform = camera.AddComponent<Canis::TransformComponent>();
            camTransform.registry = &(GetScene().entityRegistry); 
            Canis::SetTransformPosition(camTransform, glm::vec3(1.0f, 3.0f, 1.0f));
            PlayerCamera& playerCamera = camera.AddScript<PlayerCamera>();
            playerCamera.target = monkey;
            
            camera.SetParent(cameraPivot);
            playerMovement.camera = camera;
            playerMovement.cube = cube;
        }

        Canis::Entity turtle = CreateEntity();

        {
            mesh.material = Canis::AssetManager::LoadMaterial("assets/materials/yes_default.material");
            Canis::SetTransformPosition(transform, glm::vec3(0.6f));
            Canis::SetTransformScale(transform, glm::vec3(0.2f, 0.2f, 0.2f));
            mesh.id = Canis::AssetManager::LoadModel("assets/models/turtle.obj");
            turtle.AddComponent<Canis::TransformComponent>(transform);
            turtle.AddComponent<Canis::ColorComponent>();
            turtle.AddComponent<Canis::MeshComponent>(mesh);
            turtle.AddComponent<Canis::SphereColliderComponent>(collider);
            Turtle& tur = turtle.AddScript<Turtle>();
            tur.player = monkey;
        }

        Canis::Entity chicken = CreateEntity();

        {
            Canis::SetTransformPosition(transform, glm::vec3(1.5f));
            Canis::SetTransformScale(transform, glm::vec3(0.2f, 0.2f, 0.2f));
            mesh.id = Canis::AssetManager::LoadModel("assets/models/bird.obj");
            chicken.AddComponent<Canis::TransformComponent>(transform);
            chicken.AddComponent<Canis::ColorComponent>();
            chicken.AddComponent<Canis::MeshComponent>(mesh);
            chicken.AddComponent<Canis::SphereColliderComponent>(collider);
            Chicken& chick = chicken.AddScript<Chicken>();
            chick.player = monkey;
        }
        
        Canis::Entity sharkEntity = CreateEntity();

        {
            mesh.material = Canis::AssetManager::LoadMaterial("assets/materials/yes_default.material");
            Canis::SetTransformPosition(transform, glm::vec3(1.5f, 0.0f, 1.5f));
            Canis::SetTransformScale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
            mesh.id = Canis::AssetManager::LoadModel("assets/models/shark.glb");
            sharkEntity.SetTag("SHARK");
            sharkEntity.AddComponent<Canis::TransformComponent>(transform);
            sharkEntity.AddComponent<Canis::ColorComponent>();
            sharkEntity.AddComponent<Canis::MeshComponent>(mesh);
            sharkEntity.AddComponent<Canis::SphereColliderComponent>(collider);
            Shark& shark = sharkEntity.AddScript<Shark>();
            shark.raft = cube;

            /*Canis::Entity sharkEntityG = CreateEntity();

            mesh.material = Canis::AssetManager::LoadMaterial("assets/materials/yes_default.material");
            Canis::SetTransformPosition(transform, glm::vec3(0.0f));
            Canis::SetTransformScale(transform, glm::vec3(0.55f, 0.55f, 0.11f));
            mesh.id = Canis::AssetManager::LoadModel("assets/models/shark.obj");
            sharkEntityG.AddComponent<Canis::TransformComponent>(transform);
            sharkEntityG.AddComponent<Canis::ColorComponent>();
            sharkEntityG.AddComponent<Canis::MeshComponent>(mesh);
            sharkEntityG.AddComponent<Canis::SphereColliderComponent>(collider);

            sharkEntityG.SetParent(sharkEntity);*/
        }
    }
    
    void OnDestroy()
    {

    }

    void OnUpdate(float _dt)
    {
        
    }
};

bool DecodeSpawnCube(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "SpawnCube")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<SpawnCube>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}