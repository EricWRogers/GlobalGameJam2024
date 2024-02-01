#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 MODEL;
uniform mat4 lightSpaceMatrix;
uniform mat4 boneTransforms[32];

void main()
{
    vec4 totalPosition = vec4(0.0f);
    bool useBone = false;

    for(int i = 0 ; i < 4 ; i++)
    {
        if(aBoneIDs[i] == -1) 
            continue;
        
        useBone = true;

        if(aBoneIDs[i] >= 32) 
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = boneTransforms[aBoneIDs[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * aWeights[i];
        //vec3 localNormal = mat3(boneTransforms[aBoneIDs[i]]) * norm;
    }

    /*mat4 boneTransform = boneTransforms[aBoneIDs[0]] * aWeights[0];

    if (aBoneIDs[0] != -1)
    {
        if (aBoneIDs[1] != -1)
            boneTransform += boneTransforms[aBoneIDs[1]] * aWeights[1];
        if (aBoneIDs[2] != -1)
            boneTransform += boneTransforms[aBoneIDs[2]] * aWeights[2];
        if (aBoneIDs[3] != -1)
            boneTransform += boneTransforms[aBoneIDs[3]] * aWeights[3];
        
        vs_out.FragPos = vec3(MODEL * boneTransform * vec4(aPos, 1.0));
        gl_Position = projection * view * MODEL * boneTransform * vec4(aPos, 1.0);
    }*/

    if (aBoneIDs[0] != -1)
    {
        vs_out.FragPos = vec3(MODEL * totalPosition);
        gl_Position = projection * view * MODEL * totalPosition;
    }
    else
    {
        vs_out.FragPos = vec3(MODEL * vec4(aPos, 1.0));
        gl_Position = projection * view * MODEL * vec4(aPos, 1.0);
    }

    vs_out.Normal = transpose(inverse(mat3(MODEL))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}
