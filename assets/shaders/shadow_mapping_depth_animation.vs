#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 boneTransforms[64];

void main()
{
    if (aBoneIDs[0] != -1)
    {
        mat4 boneTransform = boneTransforms[aBoneIDs[0]] * aWeights[0];

        if (aBoneIDs[1] != -1)
            boneTransforms[aBoneIDs[1]] * aWeights[1];
        if (aBoneIDs[2] != -1)
            boneTransforms[aBoneIDs[2]] * aWeights[2];
        if (aBoneIDs[3] != -1)
            boneTransforms[aBoneIDs[3]] * aWeights[3];
        
        gl_Position = lightSpaceMatrix * MODEL * boneTransform * vec4(aPos, 1.0);
    }
    else
    {
        gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    }
}