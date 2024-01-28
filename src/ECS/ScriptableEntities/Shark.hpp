#pragma once
#include <SDL_keyboard.h>
#include <string>

#include <glm/gtx/rotate_vector.hpp>

#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/ScriptComponent.hpp>
#include <Canis/ECS/Components/TransformComponent.hpp>
#include <Canis/SceneManager.hpp>

class Shark : public Canis::ScriptableEntity
{
public:
    Canis::Entity raft;
    vec3 direction;
    float speed = 5.0f;

    float spawnMinRadius = 15.0f;
    float spawnMaxRadius = 20.0f;

    float raftOffset = 0.5f;

    void OnCreate() {}

    void OnReady() {
        ResetPosition();
    }

    void OnDestroy() {}

    bool TooFarCheck() {
        using namespace Canis;
        using namespace glm;

        TransformComponent &transform = GetComponent<TransformComponent>();
        vec3 position = GetGlobalPosition(transform);
        vec3 center = vec3(0.0f);
        center.y = position.y;
        return (distance(center, position) > spawnMaxRadius);
    }

    bool CheckForRaft()
    {
        using namespace Canis;
        using namespace glm;

        TransformComponent &transform = GetComponent<TransformComponent>();

        vec3 raftPos = GetGlobalPosition(raft.GetComponent<TransformComponent>());
        vec3 sharkPos = GetGlobalPosition(GetComponent<TransformComponent>());
        raftPos.y = sharkPos.y;
        vec3 dir = normalize(raftPos - sharkPos);
        

        Hit raftHit;
        Hit sharkHit;

        Ray ray(sharkPos, dir);

        if (CheckRay(entity, ray, sharkHit))
        {
            if (CheckRay(raft, ray, raftHit))
            {
                float raftHitDistance = distance(raftHit.position, sharkPos);
                float sharkHitDistance = distance(sharkHit.position, sharkPos);

                

                if (raftHitDistance < sharkHitDistance - raftOffset)
                    return true;
            }
        }

        return false;
    }

    void ResetPosition() {
        using namespace Canis;
        using namespace glm;

        TransformComponent &transform = GetComponent<TransformComponent>();

        vec3 randomPos = GetGlobalPosition(transform);

        while (true)
        {
            randomPos.x = RandomFloat(-spawnMaxRadius, spawnMaxRadius);
            randomPos.z = RandomFloat(-spawnMaxRadius, spawnMaxRadius);

            if ((distance(randomPos, vec3(0.0f, randomPos.y, 0.0f)) < spawnMaxRadius) && (distance(randomPos, vec3(0.0f, randomPos.y, 0.0f)) > spawnMinRadius))
            {
                SetTransformPosition(transform, randomPos);

                vec3 raftPos = GetGlobalPosition(raft.GetComponent<TransformComponent>());
                vec3 sharkPos = GetGlobalPosition(GetComponent<TransformComponent>());

                raftPos.y = sharkPos.y;

                direction = normalize(raftPos - sharkPos);

                return;
            }
        }
    }

    void OnUpdate(float _dt)
    {
        using namespace Canis;
        using namespace glm;
        
        if (CheckForRaft())
        {
            // damage the raft
        }
        else
        {
            TransformComponent &transform = GetComponent<TransformComponent>();
            vec3 raftPos = GetGlobalPosition(raft.GetComponent<TransformComponent>());

            SetTransformPosition(transform, GetGlobalPosition(transform) + (direction * speed * _dt));
            LookAt(transform, raftPos, glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
};

bool DecodeShark(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "Shark")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<Shark>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}