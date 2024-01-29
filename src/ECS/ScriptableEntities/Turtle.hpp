#pragma once
#include <SDL_keyboard.h>
#include <string>

#include <glm/gtx/rotate_vector.hpp>

#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/ScriptComponent.hpp>
#include <Canis/ECS/Components/TransformComponent.hpp>
#include <Canis/SceneManager.hpp>

class Turtle : public Canis::ScriptableEntity
{
public:
    Canis::Entity player;
    vec3 direction;
    float speed = 5.0f;

    float spawnMinRadius = 15.0f;
    float spawnMaxRadius = 20.0f;

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

    bool CheckPlayerHit()
    {
        using namespace Canis;
        using namespace glm;

        TransformComponent &transform = GetComponent<TransformComponent>();

        vec3 playerPos = GetGlobalPosition(player.GetComponent<TransformComponent>());
        vec3 turtlePos = GetGlobalPosition(GetComponent<TransformComponent>());
        playerPos.y = turtlePos.y;
        vec3 dir = normalize(playerPos - turtlePos);
        

        Hit playerHit;
        Hit turtleHit;

        Ray ray(turtlePos, dir);

        if (CheckRay(entity, ray, turtleHit))
        {
            if (CheckRay(player, ray, playerHit))
            {
                float playerHitDistance = distance(playerHit.position, turtlePos);
                float turtleHitDistance = distance(turtleHit.position, turtlePos);

                if (playerHitDistance < turtleHitDistance)
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

                vec3 playerPos = GetGlobalPosition(player.GetComponent<TransformComponent>());
                vec3 turtlePos = GetGlobalPosition(GetComponent<TransformComponent>());

                playerPos.y = turtlePos.y;

                direction = normalize(playerPos - turtlePos);

                return;
            }
        }
    }

    void OnUpdate(float _dt)
    {
        using namespace Canis;
        using namespace glm;

        TransformComponent &transform = GetComponent<TransformComponent>();

        SetTransformPosition(transform, GetGlobalPosition(transform) + (direction * speed * _dt));
        //Canis::Rotate(transform, vec3(0.0f, radians(360.0f * _dt), 0.0f));
        if (TooFarCheck())
            ResetPosition();
        
        // check if you hit player
        if (CheckPlayerHit())
        {
            ((SceneManager*)GetScene().sceneManager)->Load("game_over");
        }
    }
};

bool DecodeTurtle(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "Turtle")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<Turtle>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}