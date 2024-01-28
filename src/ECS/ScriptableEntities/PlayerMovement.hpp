#pragma once
#include <SDL_keyboard.h>
#include <string>

#include <glm/gtx/rotate_vector.hpp>

#include <Canis/Math.hpp>
#include <Canis/ScriptableEntity.hpp>
#include <Canis/ECS/Components/ScriptComponent.hpp>
#include <Canis/ECS/Components/TransformComponent.hpp>
#include <Canis/SceneManager.hpp>

class PlayerMovement : public Canis::ScriptableEntity
{
public:
    Canis::Entity camera;
    Canis::Entity cube;

    float speed = 2.0f;
    float gravity = -25.0f;
    float jumpForce = 15.0f;
    float groundLevel = 0.0f;
    float yOffset = 0.5f;
    float turnSpeed = 5.0f;

    bool isGrounded = false;
    bool isWalking = false;

    vec3 acceleration = vec3(0.0f);

    void OnCreate() {}

    void OnReady() {}

    void OnDestroy() {}

    void OnUpdate(float _dt)
    {
        using namespace Canis;
        using namespace glm;

        isWalking = false;

        TransformComponent& transform = GetComponent<TransformComponent>();

        vec3 cameraPosition = GetCamera().Position;
        cameraPosition.y = transform.position.y;

        vec3 lookFront = normalize(cameraPosition - transform.position);
        vec3 lookRight = glm::cross(vec3(0.0f, 1.0f, 0.0f), lookFront);

        vec3 targetPosition = transform.position;

        if (GetInputManager().GetKey(SDL_SCANCODE_BACKSPACE) || GetInputManager().GetKey(SDL_SCANCODE_SPACE))
            if (isGrounded)
                acceleration.y = jumpForce;

        acceleration.y += gravity * _dt;

        targetPosition.y += acceleration.y * _dt;

        Hit hit;
        if (CheckRay(cube, Ray(GetGlobalPosition(transform), vec3(0.0f, -1.0f, 0.0f)), hit))
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

        
        

        if (GetInputManager().GetKey(SDL_SCANCODE_W))
        {
            targetPosition -= lookFront * speed * _dt;
            isWalking = true;
        }

        if (GetInputManager().GetKey(SDL_SCANCODE_S))
        {
            targetPosition += lookFront * speed * _dt;
            isWalking = true;
        }

        if (GetInputManager().GetKey(SDL_SCANCODE_A))
        {
            targetPosition -= lookRight * speed * _dt;
            isWalking = true;
        }

        if (GetInputManager().GetKey(SDL_SCANCODE_D))
        {
            targetPosition += lookRight * speed * _dt;
            isWalking = true;
        }

        if (isWalking)
        {
            RotateTowardsLookAt(transform, transform.position + lookFront, vec3(0.0f, 1.0f, 0.0f), turnSpeed * _dt);
        }

        // OnGround Edge Check

        SetTransformPosition(transform, targetPosition);

        // check if you fell in water
        if (GetGlobalPosition(transform).y < 0.0f)
        {
            ((SceneManager*)GetScene().sceneManager)->Load("game_over");
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