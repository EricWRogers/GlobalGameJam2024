#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aModelInstance;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * aModelInstance * vec4(aPos, 1.0);
}