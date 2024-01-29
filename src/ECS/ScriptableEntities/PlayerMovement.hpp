#pragma once
#include <SDL_keyboard.h>
#include <string>

#include <glm/gtx/rotate_vector.hpp>

#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/ScriptComponent.hpp>
#include <Canis/ECS/Components/UISliderComponent.hpp>
#include <Canis/ECS/Components/TransformComponent.hpp>
#include <Canis/SceneManager.hpp>

#include <Canis/ECS/Components/MeshComponent.hpp>

#include "Shark.hpp"

class PlayerMovement : public Canis::ScriptableEntity
{
public:
    Canis::Entity camera;
    Canis::Entity cube;
    Canis::Entity healthBar;

    float speed = 2.0f;
    float gravity = -25.0f;
    float jumpForce = 15.0f;
    float groundLevel = 0.0f;
    float yOffset = 0.5f;
    float turnSpeed = 5.0f;

    float raftMaxSize = 5.0f;

    bool isGrounded = false;
    bool isWalking = false;

    vec3 acceleration = vec3(0.0f);

    void OnCreate() {}

    void OnReady()
    {
        yOffset = entity.GetComponent<Canis::TransformComponent>().scale.y / 2.0f;
        healthBar = entity.GetEntityWithTag("HealthSlider");
    }

    void OnDestroy() {}

    bool StompSharkCheck()
    {
        using namespace Canis;
        using namespace glm;

        bool found = false;

        std::vector<Entity> sharks = entity.GetEntitiesWithTag("SHARK");

        for (Entity shark : sharks)
        {
            MeshComponent& mesh = shark.GetComponent<MeshComponent>();
            //mesh.id = Canis::AssetManager::LoadModel("assets/models/shark_hit_box.obj");
            //SetTransformScale(shark.GetComponent<TransformComponent>(), glm::vec3(1.0f, 1.0f, 5.0f));
            vec3 playerPos = GetGlobalPosition(GetComponent<TransformComponent>());
            vec3 sharkPos = GetGlobalPosition(shark.GetComponent<TransformComponent>());


            vec3 dir = normalize(sharkPos - playerPos);

            Hit playerHit;
            Hit sharkHit;

            Ray ray(playerPos, dir);

            if (CheckRay(entity, ray, playerHit))
            {
                if (CheckRay(shark, ray, sharkHit))
                {
                    float playerHitDistance = distance(playerHit.position, playerPos);
                    float sharkHitDistance = distance(sharkHit.position, playerPos);

                    if (playerHitDistance + 0.8f > sharkHitDistance)
                    {
                        static_cast<Shark *>(shark.GetComponent<ScriptComponent>().Instance)->ResetPosition();
                        found = true;
                    }
                }
            }

            //mesh.id = Canis::AssetManager::LoadModel("assets/models/shark.obj");
            //SetTransformScale(shark.GetComponent<TransformComponent>(), glm::vec3(1.0f, 1.0f, 1.0f));
        }

        return found;
    }

    void OnUpdate(float _dt)
    {
        using namespace Canis;
        using namespace glm;

        isWalking = false;
        bool wasGrouded = isGrounded;

        TransformComponent &transform = GetComponent<TransformComponent>();

        vec3 cameraPosition = GetCamera().Position;
        cameraPosition.y = transform.position.y;

        vec3 lookFront = normalize(cameraPosition - transform.position);
        vec3 lookRight = glm::cross(vec3(0.0f, 1.0f, 0.0f), lookFront);

        vec3 targetPosition = transform.position;

        Hit hit;
        if (FindRayMeshIntersection(cube, Ray(GetGlobalPosition(transform), vec3(0.0f, -1.0f, 0.0f)), hit))
        {
            if (hit.position.y >= targetPosition.y - yOffset)
            {
                isGrounded = true;
                targetPosition.y = yOffset + hit.position.y;
                acceleration.y = 0.0f;
            }
            else
            {
                isGrounded = false;
            }
        }
        else
        {
            isGrounded = false;
        }

        if (isGrounded == false) // || wasGrouded == false)
        {
            isGrounded = StompSharkCheck();
        }

        if (isGrounded == false)
        {
            if (GetInputManager().JustPressedKey(SDLK_BACKSPACE) || 
                GetInputManager().JustPressedKey(SDLK_SPACE) ||
                GetInputManager().JustPressedButton(0, Canis::ControllerButton::A) ||
                GetInputManager().JustPressedButton(0, Canis::ControllerButton::RIGHTSHOULDER) ||
                GetInputManager().JustLeftClicked())
                acceleration.y = -jumpForce;
        }

        if (isGrounded)
            acceleration.y = jumpForce;

        acceleration.y += gravity * _dt;

        targetPosition.y += acceleration.y * _dt;

        vec2 inputVec = vec2(0.0f);

        inputVec = GetInputManager().GetRightStick(0);
        inputVec.y *= -1;

        if (GetInputManager().GetKey(SDL_SCANCODE_W))
            inputVec.y = -1;

        if (GetInputManager().GetKey(SDL_SCANCODE_S))
            inputVec.y = 1;

        if (GetInputManager().GetKey(SDL_SCANCODE_A))
            inputVec.x = -1;

        if (GetInputManager().GetKey(SDL_SCANCODE_D))
            inputVec.x = 1;

        if (inputVec != vec2(0.0f))
        {
            isWalking = true;
            targetPosition += lookFront * (inputVec.y * speed * _dt);
            targetPosition += lookRight * (inputVec.x * speed * _dt);
        }

        if (isWalking)
        {
            RotateTowardsLookAt(transform, transform.position + lookFront, vec3(0.0f, 1.0f, 0.0f), turnSpeed * _dt);
        }

        // OnGround Edge Check

        SetTransformPosition(transform, targetPosition);

        UISliderComponent &uiSlider = healthBar.GetComponent<UISliderComponent>();

        float raftScale = raftMaxSize * uiSlider.value;

        Clamp(raftScale, 0.8f, raftMaxSize);

        cube.SetScale(glm::vec3(raftScale, 0.8f, raftScale));

        // check if you fell in water
        if (GetGlobalPosition(transform).y < 0.0f)
        {
            ((SceneManager *)GetScene().sceneManager)->Load("game_over");
        }
    }
};

bool DecodePlayerMovement(const std::string &_name, Canis::Entity &_entity)
{
    if (_name == "PlayerMovement")
    {
        Canis::ScriptComponent scriptComponent = {};
        scriptComponent.Bind<PlayerMovement>();
        _entity.AddComponent<Canis::ScriptComponent>(scriptComponent);
        return true;
    }
    return false;
}