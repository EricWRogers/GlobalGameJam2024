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

class SpawnCube : public Canis::ScriptableEntity
{
public:
    void OnCreate()
    {
        
    }

    void OnReady()
    {
        Canis::TransformComponent t;
        t.registry = &(GetScene().entityRegistry);

        Canis::Entity emptyParent = CreateEntity();
        emptyParent.AddComponent<Canis::TransformComponent>(t);
        emptyParent.AddScript<Rotate>();
        

        Canis::TransformComponent transform;
        transform.registry = &(GetScene().entityRegistry);
        transform.position.x += 3;

        Canis::MeshComponent mesh;
        mesh.id = Canis::AssetManager::LoadModel("assets/models/white_block.obj");
        mesh.material = Canis::AssetManager::LoadMaterial("assets/materials/box.material");

        Canis::SphereColliderComponent collider;

        Canis::Entity cube = CreateEntity();
        cube.AddComponent<Canis::TransformComponent>(transform);
        cube.AddComponent<Canis::ColorComponent>();
        cube.AddComponent<Canis::MeshComponent>(mesh);
        cube.AddComponent<Canis::SphereColliderComponent>(collider);
        cube.AddScript<Rotate>();

        cube.SetParent(emptyParent);

        Canis::Entity smallCube = CreateEntity();
        smallCube.AddComponent<Canis::TransformComponent>(transform);
        smallCube.AddComponent<Canis::ColorComponent>();
        smallCube.AddComponent<Canis::MeshComponent>(mesh);
        smallCube.AddComponent<Canis::SphereColliderComponent>(collider);

        smallCube.SetPosition(glm::vec3(0.5f));
        smallCube.SetScale(glm::vec3(0.5f));
        smallCube.SetParent(cube);

        Canis::Entity smallSmallCube = CreateEntity();
        smallSmallCube.AddComponent<Canis::TransformComponent>(transform);
        smallSmallCube.AddComponent<Canis::ColorComponent>();
        smallSmallCube.AddComponent<Canis::MeshComponent>(mesh);
        smallSmallCube.AddComponent<Canis::SphereColliderComponent>(collider);

        smallSmallCube.SetPosition(glm::vec3(0.5f));
        smallSmallCube.SetScale(glm::vec3(0.5f));
        smallSmallCube.SetParent(smallCube);

        Canis::Entity monkey = CreateEntity();

        {
            Canis::TransformComponent tr;
            tr.registry = &(GetScene().entityRegistry);    

            mesh.id = Canis::AssetManager::LoadModel("assets/models/monkey.obj");        

            monkey.AddComponent<Canis::TransformComponent>(tr);
            monkey.AddComponent<Canis::ColorComponent>();
            monkey.AddComponent<Canis::MeshComponent>(mesh);
            monkey.AddComponent<Canis::SphereColliderComponent>(collider);
            
        }

        PlayerMovement& playerMovement = monkey.AddScript<PlayerMovement>();

        {
            Canis::Entity cameraPivot = CreateEntity();
            Canis::TransformComponent& camPivotTransform = cameraPivot.AddComponent<Canis::TransformComponent>();
            camPivotTransform.registry = &(GetScene().entityRegistry); 

            Canis::Entity camera = CreateEntity();
            Canis::TransformComponent& camTransform = camera.AddComponent<Canis::TransformComponent>();
            camTransform.registry = &(GetScene().entityRegistry); 
            Canis::SetTransformPosition(camTransform, glm::vec3(3.0f));
            PlayerCamera& playerCamera = camera.AddScript<PlayerCamera>();
            playerCamera.target = monkey;
            
            camera.SetParent(cameraPivot);
            playerMovement.camera = camera;
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